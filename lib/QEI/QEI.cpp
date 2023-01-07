#include <stdio.h>
#include <cstdlib> 
#include <cmath>
#include "QEI.hpp"
#include <fbienc.h>

namespace ntlab {

bool QEI::open()
{
  int nRet = EncOpen( id_, ENC_FLAG_NORMAL );
  if ( nRet != ENC_ERROR_SUCCESS ) {
    printf("failed to open\r\n");
    return false;
  }
  return true;
}

bool QEI::set_channel_config(void)
{
  pulse_per_revolution = (unsigned long*)malloc(sizeof(unsigned long) * channels_);
  ulSetCounter         = (unsigned long*)malloc(sizeof(unsigned long) * channels_);
  ulGetCounter         = (unsigned long*)malloc(sizeof(unsigned long) * channels_);
  radians_             = (double*)malloc(sizeof(double) * channels_);
  enc_mode             = (int*)malloc(sizeof(int) * channels_);
  enc_mode[0]=0;
  enc_mode[1]=1;
  enc_mode[2]=0;
  enc_mode[3]=0;
  for (int i = 0; i < channels_; i++) {
    int nRet = EncSetSystemMode( id_, i+1, 0);
    if ( nRet != ENC_ERROR_SUCCESS ) {
      printf("failed to set system mode %d\r\n", i);
      return false;
    }
    nRet = EncSetMode( id_, i+1, 6, enc_mode[i], 0);
    if ( nRet != ENC_ERROR_SUCCESS ) {
      printf("failed to set encoder mode\r\n");
      return false;
    }
  }
  for (int i = 0; i < channels_; i++) {
    ulSetCounter[i] = 0x000000;
    pulse_per_revolution[i] = ppr_;
  }
  int nRet = EncSetCounterEx( id_, 0x0f, ulSetCounter );
  if ( nRet != ENC_ERROR_SUCCESS ) {
    printf("failed to set encoder counter\r\n");
    return false;
  }
  return true;
}

bool QEI::set_mode(unsigned char ch, int mod)
{
  if(ch < 0 || ch >= channels_) return false;
  enc_mode[ch] = mod;
  EncSetMode( id_, ch+1, 6, enc_mode[ch], 0);
  return true;
}

bool QEI::set_pulse(unsigned char ch, unsigned long pulse)
{
  if(ch < 0 || ch >= channels_) return false;
  if(pulse < 0) return false;

  pulse_per_revolution[ch] = pulse;
  return true;
}

int QEI::read(void)
{
  int nRet = EncGetCounterEx( id_, 0x0f, ulGetCounter );
  if ( nRet != ENC_ERROR_SUCCESS ){
    printf("failed to get encoder counter\r\n");
    return false;
  }
  for (int i = 0; i < channels_; i++) {
    signed long slGetCounter = (signed long)(ulGetCounter[i]<<8)>>8;
    radians_[i] = 2.0 * M_PI * (double)slGetCounter / pulse_per_revolution[i];
  }
  return true;
}

bool QEI::close(void)
{
  int nRet = EncClose(id_);
  if ( nRet != ENC_ERROR_SUCCESS )
  {
    printf("failed to close\r\n");
    return false;
  }
  return true;
}

}
