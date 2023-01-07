#include "DAC.hpp"
#include <stdio.h>
#include <cstdlib> 
#include <fbida.h>

namespace ntlab {

DAC::DAC(int id, int channels)
{
  id_ = id;
  channels_ = channels;
}

bool DAC::open(void)
{
	int nRet = DaOpen( id_ );
	if ( nRet != DA_ERROR_SUCCESS ) {
		printf("cannot open DAC Error:%x\n", nRet);
		return false;
	}
	return true;
}
void DAC::dump_spec(void)
{
	DABOARDSPEC bs;
	int nRet = DaGetDeviceInfo(id_, &bs);
	printf("nRet = %d\n", nRet);
	printf("BoardType   : %ld\n",bs.ulBoardType);
	printf("BoardID     : %ld\n",bs.ulBoardID);
	printf("SamplingMode: %ld\n",bs.ulSamplingMode);
	printf("Resolution  : %ld\n",bs.ulResolution);
	printf("ChCount     : %ld\n",bs.ulChCount);
	printf("Range       : %lx\n",bs.ulRange);
	printf("Isolation   : %ld\n",bs.ulIsolation);
	printf("DI          : %ld\n",bs.ulDi);
	printf("DO          : %ld\n",bs.ulDo);
}
int DAC::close(void)
{ 
	int nRet = DaClose(id_);
	if ( nRet != DA_ERROR_SUCCESS )
		return false;
	return true;
}
void DAC::set_channel_config(void)
{
  DaSmplChReq = (DASMPLCHREQ*)malloc(sizeof(DASMPLCHREQ) * channels_); 
  for(int ch = 0; ch < channels_; ch++){
    DaSmplChReq[ch].ulChNo = ch+1;
    DaSmplChReq[ch].ulRange = DA_10V;
  }
}
bool DAC::output_single(Channel ch)
{  
  if(ch.voltage >  ch.limit_voltage) ch.voltage =  ch.limit_voltage;
  if(ch.voltage < -ch.limit_voltage) ch.voltage = -ch.limit_voltage;
  unsigned short *buf = (unsigned short*)malloc(sizeof(unsigned short) * channels_);
  if(ch.channel >= channels_ || ch.channel < 0)
  {
    printf("channel number is wrong!!!\r\n");
    return false;
  }
  buf[ch.channel] = (unsigned short)((ch.voltage/20.0)*0xffff+0x8000);
  int nRet = DaOutputDA(id_, channels_, DaSmplChReq, buf);
  return true;
}

} // ntlab