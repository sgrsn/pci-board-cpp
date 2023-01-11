#include <stdio.h>
#include <lib/ADC/adc.hpp>
#include <unistd.h>
#include "../expart/common/basic.h"

int main(void)
{
	ntlab::ADC adc(1, 4);
	adc.open();
	adc.set_channel_config();
	adc.dump_spec();
  do{
		double v = adc.read_single(0);
		double torque = v / 5.0 * 20.0;
		printf("%8.3f[V], %8.3f[Nm]\n", v, torque);
		cur_up(1);
	}while( !kbhit_linux() );
	adc.close();
	return(0);
}
