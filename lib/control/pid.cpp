#include "pid.hpp"
#include <time.h>
#include <cmath>

namespace ntlab {

const double& PID::control(const double& ref, const double& cur)
{
  clock_gettime(CLOCK_MONOTONIC, &ts_[0]);
  double dt = (((double)ts_[0].tv_sec + (double)ts_[0].tv_nsec*0.000000001) - ((double)ts_[1].tv_sec + (double)ts_[1].tv_nsec*0.000000001));
  ts_[1] = ts_[0];

  double err2 = (ref - cur) / dt;
  double err0 = ref - cur;
  double err1 = err0 * dt;
  if(!std::isnan(err2))
    err_[2] = err2;
  if(!std::isnan(err1))
    err_[1] += err1;
  if(!std::isnan(err0))
    err_[0] = err0;

  // control control input
  control_input_[0] = param_.kp*err_[0] + param_.ki*err_[1] + param_.kd*err_[2];
  // update
  control_input_[1] = control_input_[0];

  return control_input_[0];
}

} // ntlab