#ifndef LIB_NTLAB_DIFFERENTIATOR_HPP
#define LIB_NTLAB_DIFFERENTIATOR_HPP

#include <vector>
#include <time.h>

namespace ntlab
{

template <class T>
class Differentiator
{
public:
  Differentiator()
  {
    data_.resize(2);
    ts_.resize(2);
    init();
  };
  
  void init()
  {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    ts_[0] = ts;
    ts_[1] = ts;
  }

  T update(T input)
  {
    clock_gettime(CLOCK_MONOTONIC, &ts_[0]);

    data_[0] = input;
    T error = data_[0] - data_[1];

    T diff = error / (((double)ts_[0].tv_sec + (double)ts_[0].tv_nsec*0.000000001) - ((double)ts_[1].tv_sec + (double)ts_[1].tv_nsec*0.000000001));

    data_[1] = data_[0];
    ts_[1] = ts_[0];

    return diff;
  }

private:
  std::vector<T> data_;
  std::vector<timespec> ts_;

};  // Differentiator

} // ntlab

  
#endif // LIB_NTLAB_DIFFERENTIATOR_HPP