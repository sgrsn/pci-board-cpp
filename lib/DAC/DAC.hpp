#ifndef LIB_NTLAB_DAC_HPP
#define LIB_NTLAB_DAC_HPP

#include <fbida.h>

namespace ntlab {

class DAC
{
 public:
  struct Channel{
    int channel;
    double voltage; 
    double limit_voltage;
  };

  DAC() { };
  DAC(int id, int channels);

  // setter
  void set_board_id(int id) { id_ = id; };
  void set_channels(int n) { channels_ = n; };

  bool open(void);
  void dump_spec(void);
  int close(void);
  void set_channel_config(void);
  bool output_single(Channel channels);

private:
  int id_;
  int channels_;
  DASMPLCHREQ* DaSmplChReq;
};  // DAC

} // ntlab

  
#endif // LIB_NTLAB_DAC_HPP