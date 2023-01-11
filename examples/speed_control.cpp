#include <stdio.h>
#include <lib/QEI/QEI.hpp>
#include <lib/DAC/DAC.hpp>
#include "../expart/common/basic.h"
#include <lib/Diff/differentiator.hpp>
#include <lib/control/pid.hpp>
#include <lib/eggx_plot/plot.hpp>
#include <vector>
#include <lib/eggx_plot/plot.hpp>

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

  ntlab::PID::Parameter pid_param{0.001, 0.05, 0.0};
  ntlab::PID pid(pid_param);
  double ref_speed = 100.0;
  double limit_voltage = 4.0;
  double rad_per_sec = 0;

  ntlab::Plot plot(1000);
  plot.setYLim(0, 200);
  int time = 0;

  do{
		qei.read();
    rad_per_sec = diff.update(qei.radian(0));
    double u = pid.control(ref_speed, rad_per_sec);
    ntlab::DAC::Channel ch{1, u, limit_voltage};
    dac.output_single(ch);
    printf("%8.3f[rad]  %8.3f[rad/s], %8.3f[V]\n", qei.radian(0), rad_per_sec, u);
		cur_up(1);
    
    plot.clear();
    plot.updatePlot(time++, rad_per_sec);
    plot.drawAutoAxisX();
    plot.draw();
	}while( !kbhit_linux() );

  plot.close();
	qei.close();
	dac.close();

	return(0);
}
