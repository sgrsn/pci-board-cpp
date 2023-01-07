#include <stdio.h>
#include <lib/QEI/QEI.hpp>
#include "../expart/common/basic.h"

int main(void)
{
	ntlab::QEI qei(1, 4, 4000);
  qei.open();
  qei.set_channel_config();
  qei.set_mode(0, MODE_DOWN);
	qei.set_pulse(0, 4000);

  do{
		qei.read();
		for(int i = 0; i < 4; i++){
			printf("%2dch: %08lx[pulse]  %8.3f[rad]\n", i+1, qei.pulse(i), qei.radian(i) );
		}
		cur_up(4);
	}while( !kbhit_linux() );

	qei.close();

	return(0);
}
