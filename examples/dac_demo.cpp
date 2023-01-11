#include <stdio.h>
#include <lib/DAC/DAC.hpp>
#include <unistd.h>

int main(void)
{
	ntlab::DAC dac(1, 4);
	
	dac.open();
	dac.set_channel_config();
	dac.dump_spec();
	ntlab::DAC::Channel ch{1, 0.3, 8.0};
	dac.output_single(ch);
	sleep(2);
	dac.close();

	return(0);
}
