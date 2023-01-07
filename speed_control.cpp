#include <stdio.h>
#include <lib/QEI/QEI.hpp>
#include <lib/DAC/DAC.hpp>
#include "../expart/common/basic.h"
#include <lib/Diff/differentiator.hpp>
#include <vector>

int main(void)
{
  ntlab::Differentiator<double> diff;
	ntlab::QEI qei(1, 4, 4000);
  qei.open();
  qei.set_channel_config();
  qei.set_mode(0, MODE_DOWN);
	qei.set_pulse(0, 4000);

	ntlab::DAC dac(1, 4);
	dac.open();
	dac.set_channel_config();
	dac.dump_spec();

  double ref_speed = 50.0;
  double kp = 0.01;
  double ki = 1.0;

  std::vector<timespec> ts;
  ts.resize(2);
  clock_gettime(CLOCK_MONOTONIC, &ts[1]);

  double limit_voltage = 8.0;

  double rad_per_sec = 0;
  double integral = 0;

  do{

    clock_gettime(CLOCK_MONOTONIC, &ts[0]);
    double dt = (((double)ts[0].tv_sec + (double)ts[0].tv_nsec*0.000000001) - ((double)ts[1].tv_sec + (double)ts[1].tv_nsec*0.000000001));
    ts[1] = ts[0];

		qei.read();
    rad_per_sec = diff.update(qei.radian(0))*0.5 + rad_per_sec*0.5;

    double error = ref_speed - rad_per_sec;
    integral += error * dt;
    double u = kp * error + ki * integral;

    ntlab::DAC::Channel ch{1, u, limit_voltage};
    dac.output_single(ch);

    printf("%8.3f[rad]  %8.3f[rad/s], %8.3f[V]\n", qei.radian(0), rad_per_sec, u);
		cur_up(1);
	}while( !kbhit_linux() );

	qei.close();
	dac.close();

	return(0);
}
