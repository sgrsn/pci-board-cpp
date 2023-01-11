#include "adc.hpp"
#include <stdio.h>
#include <cstdlib>
#include <fbiad.h>

namespace ntlab {

ADC::ADC(int id, int channels)
{
  id_ = id;
  channels_ = channels;
}

bool ADC::open(void)
{
  int nRet = AdOpen(id_);
  if(nRet)
  {
	  printf("AdOpen error: ret=%Xh\n", nRet);
	  return false;
  }
  return true;
}
void ADC::dump_spec(void)
{
  ADBOARDSPEC bs;
  int nRet = AdGetDeviceInfo(id_, &bs);
  printf("nRet = %d\n",nRet);
  printf("BoardType   : %ld\n",bs.ulBoardType);
  printf("BoardID     : %ld\n",bs.ulBoardID);
  printf("SamplingMode: %ld\n",bs.ulSamplingMode);
  printf("Resolution  : %ld\n",bs.ulResolution);
  printf("ChCountS    : %ld\n",bs.ulChCountS);
  printf("ChCountD    : %ld\n",bs.ulChCountD);
  printf("Range       : %lx\n",bs.ulRange);
  printf("Isolation   : %ld\n",bs.ulIsolation);
  printf("DI          : %ld\n",bs.ulDI);
  printf("DO          : %ld\n",bs.ulDO);
}
int ADC::close(void)
{ 
  int nRet = AdClose(id_);
  if ( nRet != AD_ERROR_SUCCESS ){
	  printf("AdClose error: ret=%Xh\n", nRet);
	  return true;
  }
  return true;
}
void ADC::set_channel_config(void)
{ 
  ADSMPLCHREQ AdSmplChReq[channels_];
  for(int ch = 0; ch < channels_; ch++)
  {
	  AdSmplChReq[ch].ulChNo = ch+1;
	  AdSmplChReq[ch].ulRange = AD_10V;
  }
  AdSetInputADEx(id_, channels_, AD_INPUT_SINGLE, AdSmplChReq);
}
double ADC::read_single(int ch)
{
  if(ch >= channels_ || ch < 0)
  {
    printf("channel number is wrong!!!\r\n");
    return 0;
  }
  unsigned short AdData[channels_];
  int nRet = AdInputADEx(id_, AdData);
  return ((double)AdData[ch] / 0xffff) * 20.0 - 10.0;
}

} // ntlab