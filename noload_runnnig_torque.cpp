#include <stdio.h>
#include <vector>
#include <time.h>
#include <iostream>
#include <string>
#include <math.h>

#include <lib/QEI/QEI.hpp>
#include <lib/DAC/DAC.hpp>
#include <lib/Diff/differentiator.hpp>
#include <lib/control/pid.hpp>
#include <lib/eggx_plot/plot.hpp>
#include <lib/log/log.hpp>

#include <linux/art_task.h>
#include <pthread.h>
#include <signal.h>

#include "../expart/common/basic.h"
#include "param.hpp"

bool exit_loop = false;
ntlab::QEI qei(1, 4, 4000);
ntlab::Plot plot(1000);
double rad_per_sec = 0;
double u = 0;
double brake_torque = 0.2;  //[Nm]
double ref_speed = 3000.0; //[RPM]

void *control_function(void *arg)
{
  if (art_enter(ART_PRIO_MAX, ART_TASK_PERIODIC, PERIODIC_TIME_CONTROL) != 0) {
    perror("art_enter");
  }

  ntlab::Differentiator<double> diff;

	ntlab::DAC dac(1, 4);
	dac.open();
	dac.set_channel_config();
	dac.dump_spec();

  ntlab::PID::Parameter pid_param{0.001, 0.05, 0.0};
  ntlab::PID pid(pid_param);

  while (1)
  {
    //if(art_wait() == -1){ perror("art_wait"); exit(1); }
    art_wait();
		qei.read();
    rad_per_sec = diff.update(qei.radian(0));
    u = pid.control(ref_speed*2*M_PI/60, rad_per_sec);
    ntlab::DAC::Channel ch1{MOTOR_DAC_CHANNEL, u, MOTOR_INPUT_LIMIT_VOLTAGE};
    ntlab::DAC::Channel ch2{BRAKE_DAC_CHANNEL, brake_torque*5.0, BRAKE_INPUT_LIMIT_VOLTAGE};
    dac.output_single(ch1);
    dac.output_single(ch2);
    if(exit_loop) break;
	}
  printf("thread1 exit!!\n");
	qei.close();
	dac.close();
  art_exit();
}

void *log_function(void *arg)
{
  if (art_enter(ART_PRIO_MAX, ART_TASK_PERIODIC, PERIODIC_TIME_LOG) != 0) {
    perror("art_enter");
  }
  char outstr[30];
  time_t t = time(NULL);
  strftime(outstr, sizeof(outstr), "log/log%Y%m%d%H%M%S.csv", localtime(&t));
  printf("%s\r\n", outstr);
  /*ntlab::Log log(outstr);
  std::vector<std::string> legends;
  legends.push_back("Time [sec]");
  legends.push_back("Input Angle [rad]");
  legends.push_back("Input Speed [RPM]");
  legends.push_back("Input Torque [Nm]");
  legends.push_back("Output Torque [Nm]");
  legends.push_back("Brake Torque [Nm]");
  legends.push_back("Efficiency [%]");
  log.writeLegend(legends);
  std::vector<double> data;*/
  while(1)
  {
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    double t = (double)ts.tv_sec + (double)ts.tv_nsec*0.000000001;

    /*data.push_back(t);
    data.push_back(qei.radian(0));
    data.push_back(rad_per_sec*60.0/M_PI/2);
    data.push_back(u);*/
    art_wait();
    printf("%8.3f[rad]  %8.3f[rpm], %8.3f[V]\n", qei.radian(0), rad_per_sec*60/2/M_PI, u);
    cur_up(1);
    if(exit_loop) break;
  }
  printf("thread2 exit!!\n");
  art_exit();
}

// シグナル処理関数の定義
void sig_handler(int sig)
{
  exit_loop = true;
}

int main(int argc, char* argv[])
{
  qei.open();
  qei.set_channel_config();
  qei.set_mode(0, MODE_DOWN);
	qei.set_pulse(0, 4000);
  plot.setYLim(0, 500);

  // Ctrl+Cが押されたときに生成されるシグナルを補足する関数のセットアップ
  (void) signal(SIGINT, sig_handler);

	pthread_t cfthread, gfthread;

	if(pthread_create(&cfthread, NULL, control_function, NULL)){
		perror("pthread_create");
    return 1;
	}
	if(pthread_create(&gfthread, NULL, log_function, NULL)){
		perror("pthread_create");
    return 1;
	}

  printf("main enter!!\n");
  int time = 0;
  while (1)
  {
    plot.clear();
    plot.updatePlot(time++, rad_per_sec);
    plot.drawAutoAxisX();
    plot.draw();
    if(exit_loop) break;
	}
  plot.close();
  printf("main exit!!\n");
  
	if(pthread_join(cfthread, NULL)){ perror("pthread_join"); return 1;}
	if(pthread_join(gfthread, NULL)){ perror("pthread_join"); return 1;}
	return(0);
}

