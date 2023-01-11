#ifndef LIB_NTLAB_ADC_HPP
#define LIB_NTLAB_ADC_HPP

#include <fbida.h>

namespace ntlab {

class ADC
{
 public:
  struct Channel{
    int channel;
    double voltage; 
  };

  ADC() { };
  ADC(int id, int channels);

  // setter
  void set_board_id(int id) { id_ = id; };
  void set_channels(int n) { channels_ = n; };

  bool open(void);
  void dump_spec(void);
  int close(void);
  void set_channel_config(void);
  double read_single(int ch);

private:
  int id_;
  int channels_;
  DASMPLCHREQ* DaSmplChReq;
};  // ADC

} // ntlab

  
#endif // LIB_NTLAB_ADC_HPP