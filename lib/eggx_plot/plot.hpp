#ifndef LIB_NTLAB_EGGX_PLOT_HPP
#define LIB_NTLAB_EGGX_PLOT_HPP

#include <eggxlib.h>
#include <algorithm>
#include <vector>
#include <string>
#include <iomanip>

namespace ntlab
{

class Plot
{
public:
  Plot(int n);

  void close();

  void updatePlot(double x, double y);
  void draw();

  // setter
  void setXLim(double xmin, double xmax) { xmin_=xmin; xmax_=xmax; };
  void setYLim(double ymin, double ymax) { ymin_=ymin; ymax_=ymax; };

  void setLayer();
  void clear();

  void drawAxis();
  void drawAutoAxisX();
  void drawAutoAxisY();
  void drawAutoAxis();

private:
  std::vector<std::pair<double, double> > plot_;
  int n_;
  int win_;
  double xmin_;
  double xmax_;
  double ymin_;
  double ymax_;
  bool is_layerd_;
};  // Plot

} // ntlab

  
#endif // LIB_NTLAB_EGGX_PLOT_HPP