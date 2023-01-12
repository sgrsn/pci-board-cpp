#include <stdio.h>
#include <vector>
#include <time.h>
#include <iostream>
#include <string>
#include <math.h>

#include <lib/QEI/QEI.hpp>
#include <lib/DAC/DAC.hpp>
#include <lib/ADC/adc.hpp>
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
double ref_brake_torque = 1.0;  //[Nm]
double ref_speed = 1000.0; //[RPM]

struct ExperimentState
{
  double time;
  double rad;
  double rpm;
  double input_volt;
  double input_toruqe;
  double sensor_torque;
  double brake_torque;
  double efficiency;
};

ExperimentState state;

void *control_function(void *arg)
{
  if (art_enter(ART_PRIO_MAX, ART_TASK_PERIODIC, PERIODIC_TIME_CONTROL) != 0) {
    perror("art_enter");
  }

  // Encoder
  ntlab::QEI qei(1, 4, 4000);
  qei.open();
  qei.set_channel_config();
  qei.set_mode(0, MODE_DOWN);
	qei.set_pulse(0, 4000);

  // DAC for motor and brake
	ntlab::DAC dac(1, 4);
	dac.open();
	dac.set_channel_config();
	dac.dump_spec();

  // ADC for Sensor
  ntlab::ADC adc(1, 4);
	adc.open();
	adc.set_channel_config();
	adc.dump_spec();

  double settling_time = 20.0;
  double rate = MOTOR_INPUT_LIMIT_VOLTAGE / (settling_time/4);
  
  // Get offset time
  timespec ts0;
  clock_gettime(CLOCK_MONOTONIC, &ts0);
  double t0 = (double)ts0.tv_sec + (double)ts0.tv_nsec*0.000000001;

  while (1)
  {
    art_wait();
    if(exit_loop) break;

    // Get time
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    state.time = (double)ts.tv_sec + (double)ts.tv_nsec*0.000000001 - t0;

    // Calc motor input (chopping waves)
    if(state.time < settling_time/4)
      state.input_volt = rate * state.time;
    else if(state.time < settling_time*3/4)
      state.input_volt = -rate * (state.time - settling_time/2);
    else if(state.time < settling_time)
      state.input_volt = rate * (state.time - settling_time);
    else
      state.input_volt = 0.0;

    // Set brake torque
    state.brake_torque = ref_brake_torque;

    // Get torque sensor
    double sensor_output = adc.read_single(0);
    state.sensor_torque = sensor_output / 5.0 * 20.0 - TORQUESENSOR_OFFSET;
    
    // Get Encoder
    qei.read();
    state.rad = qei.radian(0);

    ntlab::DAC::Channel ch1{MOTOR_DAC_CHANNEL, state.input_volt, MOTOR_INPUT_LIMIT_VOLTAGE, DAC_OFFSET_VOLTAGE};
    ntlab::DAC::Channel ch2{BRAKE_DAC_CHANNEL, state.brake_torque*5.0, BRAKE_INPUT_LIMIT_VOLTAGE};
    dac.output_single(ch1);
    dac.output_single(ch2);
	}
  printf("thread1 exit!!\n");
	qei.close();
	dac.close();
	adc.close();
  art_exit();
}

void *log_function(void *arg)
{
  if (art_enter(ART_PRIO_MAX, ART_TASK_PERIODIC, PERIODIC_TIME_LOG) != 0) {
    perror("art_enter");
  }
  // Log file
  char outstr[30];
  time_t t = time(NULL);
  strftime(outstr, sizeof(outstr), "log/log%Y%m%d%H%M%S.csv", localtime(&t));
  ntlab::Log log(outstr);
  std::vector<std::string> legends;
  legends.push_back("Time [sec]");
  legends.push_back("Input Angle [rad]");
  legends.push_back("Input Speed [rpm]");
  legends.push_back("Input Voltage [V]");
  legends.push_back("Output Torque [Nm]");
  legends.push_back("Brake Torque [Nm]");
  legends.push_back("Efficiency [%]");
  log.writeLegend(legends);
  std::vector<std::string>().swap(legends);

  while(1)
  {
    art_wait();
    if(exit_loop) break;

    // Dump log data
    std::vector<double> log_data;
    log_data.push_back(state.time);
    log_data.push_back(state.rad);
    log_data.push_back(state.rpm);
    log_data.push_back(state.input_volt);
    log_data.push_back(state.sensor_torque);
    log_data.push_back(state.brake_torque);
    log_data.push_back(state.efficiency);
    log.writeData(log_data);
    std::vector<double>().swap(log_data);

    // Console output
    printf("%8.3f[rad] %8.3f[V], %8.3f[Nm]\n", state.rad, state.input_volt, state.sensor_torque);
    cur_up(1);
  }
  printf("thread2 exit!!\n");
  printf("%s\r\n", outstr);
  art_exit();
}

// シグナル処理関数の定義
void sig_handler(int sig)
{
  exit_loop = true;
}

int main(int argc, char* argv[])
{
  ntlab::Plot plot(300);
  ntlab::Plot plot2(300);
  plot.setYLim(-800, 800);
  plot2.setYLim(0, 100);

  // Setup a callback function to get the Ctrl+C
  (void) signal(SIGINT, sig_handler);

  // Generate threads
	pthread_t cfthread, gfthread;

	if(pthread_create(&cfthread, NULL, control_function, NULL)){
		perror("pthread_create");
    return 1;
	}
	if(pthread_create(&gfthread, NULL, log_function, NULL)){
		perror("pthread_create");
    return 1;
	}

  // Plot visualization
  printf("main enter!!\n");
  int time = 0;
  while (1)
  {
    plot.clear();
    plot.updatePlot(time++, state.input_volt*100);
    plot.drawAutoAxisX();
    plot.draw();
    
    plot2.clear();
    plot2.updatePlot(time++, state.sensor_torque*100);
    plot2.drawAutoAxisX();
    plot2.draw();

    usleep(1000000);

    if(exit_loop) break;
	}
  plot.close();
  plot2.close();
  printf("main exit!!\n");
  
	if(pthread_join(cfthread, NULL)){ perror("pthread_join"); return 1;}
	if(pthread_join(gfthread, NULL)){ perror("pthread_join"); return 1;}
	return(0);
}

