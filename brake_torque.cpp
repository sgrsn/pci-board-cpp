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
#include <lib/filter/lowpass_filter.hpp>
#include <lib/control/pid.hpp>
#include <lib/eggx_plot/plot.hpp>
#include <lib/log/log.hpp>

#include <linux/art_task.h>
#include <pthread.h>
#include <signal.h>

#include "../expart/common/basic.h"
#include "param.hpp"

bool exit_loop = false;

struct ExperimentState
{
  double time;
  double rad;
  double rpm;
  double input_torque;
  double sensor_torque;
  double brake_torque;
  double efficiency;
};

// シグナル処理関数の定義
void sig_handler(int sig)
{
  exit_loop = true;
}

int main(int argc, char* argv[])
{
  ExperimentState state;

  // ADC for Sensor
  ntlab::ADC adc(1, 4);
	adc.open();
	adc.set_channel_config();
	adc.dump_spec();

  // Plot
  ntlab::Plot plot(1000);
  plot.setYLim(-150, 150);

  // Setup a callback function to get the Ctrl+C
  (void) signal(SIGINT, sig_handler);

  // Plot visualization
  printf("main enter!!\n");
  // Get offset time
  timespec ts0;
  clock_gettime(CLOCK_MONOTONIC, &ts0);
  double t0 = (double)ts0.tv_sec + (double)ts0.tv_nsec*0.000000001;
  int n = 1000;
  while (1)
  {
    // Get time
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    state.time = (double)ts.tv_sec + (double)ts.tv_nsec*0.000000001 - t0;
    
    double sensor_raw_sum = 0;
    for(int i = 0; i < n; i++)
    {
      // Get torque sensor
      sensor_raw_sum += adc.read_single(0);
    }
    state.sensor_torque = sensor_raw_sum/n / 5.0 * 20.0;
	  
    // Plot torque
    plot.clear();
    plot.updatePlot(state.time*100, state.sensor_torque*1000);
    plot.drawAutoAxis();
    plot.draw();

    // Console output
    printf("%8.3f[rad] %8.6f[Nm]\n", state.time, state.sensor_torque);
    cur_up(1);

    if(exit_loop) break;
	}
  adc.close();  
  plot.close();
  printf("main exit!!\n");
  
	return(0);
}

