#ifndef LIB_NTLAB_PID_HPP
#define LIB_NTLAB_PID_HPP

#include <vector>
#include <time.h>

namespace ntlab
{

class PID
{
public:
  struct Parameter {
    double kp;
    double ki;
    double kd;
  };
  PID(const Parameter& param) : param_(param)
  {
    err_.resize(3, 0);
    control_input_.resize(2, 0);
    ts_.resize(2);
    clock_gettime(CLOCK_MONOTONIC, &ts_[0]);
    clock_gettime(CLOCK_MONOTONIC, &ts_[1]);
  };
  
  // setter
  void setParameter(const Parameter& param) { param_ = param; };

  const double& control(const double& ref, const double& cur);

private:
  std::vector<timespec> ts_;
  std::vector<double> err_;
  std::vector<double> control_input_;
  Parameter param_;
};  // PID

} // ntlab

  
#endif // LIB_NTLAB_PID_HPP