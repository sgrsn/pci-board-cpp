#include <stdio.h>
#include <lib/QEI/QEI.hpp>
#include "../expart/common/basic.h"
#include <lib/Diff/differentiator.hpp>

int main(void)
{
  ntlab::Differentiator<double> diff;
	ntlab::QEI qei(1, 4, 4000);
  qei.open();
  qei.set_channel_config();
  qei.set_mode(0, MODE_DOWN);
	qei.set_pulse(0, 4000);

  do{
		qei.read();
			printf("%08lx[pulse]  %8.3f[rad]  %8.3f[rad/s]\n", qei.pulse(0), qei.radian(0), diff.update(qei.radian(0)));
		cur_up(1);
	}while( !kbhit_linux() );

	qei.close();

	return(0);
}
