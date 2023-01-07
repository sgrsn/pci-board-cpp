#ifndef LIB_NTLAB_QEI_HPP
#define LIB_NTLAB_QEI_HPP

#define MODE_UP (0)
#define MODE_DOWN (1)

namespace ntlab {


class QEI
{
 public:
  struct Channel{
    int channel;
    double radians;
    signed long counter;
    int ppr;
  };
  QEI(int id, int channels, int ppr){id_ = id; channels_ = channels; ppr_ = ppr;};

  bool open(void);
  bool close(void);
 
  bool set_channel_config(void);
  bool set_mode(unsigned char ch, int mod);
  bool set_pulse(unsigned char ch, unsigned long pulse);
  int read(void);
  
  // getter
  double radian(int ch) { 
    if(ch < 0 || ch >= channels_) return -1; 
    return radians_[ch]; 
  }; 
  
  signed long pulse(int ch) { 
    if(ch < 0 || ch >= channels_) return -1; 
	  return (signed long)(ulGetCounter[ch]<<8)>>8;
  }; 

  // setter
  void set_board_id(int id) { id_ = id; };
  void set_channels(int n) { channels_ = n; };
  void set_pulse_per_revolution(int ppr) { ppr_ = ppr; };

 private:
  int id_;
  int channels_;
  int ppr_;
  double *radians_;
  int *enc_mode;
  unsigned long *pulse_per_revolution;
  unsigned long *ulSetCounter;
  unsigned long *ulGetCounter;
};  // QEI

} // ntlab

  
#endif // LIB_NTLAB_QEI_HPP