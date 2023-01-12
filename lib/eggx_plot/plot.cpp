#include "plot.hpp"
#include <eggx.h>

namespace ntlab
{
  
Plot::Plot(int n) : n_(n)
{
  plot_.resize(n_, {0, 0});
  win_ = eggx_gopen(500, 500);
  eggx_gsetbgcolor(win_, "white"); 
  eggx_gclr(win_);
  eggx_newcolor(win_, "black");
};

void Plot::close()
{
  eggx_gclose(win_);
}

void Plot::updatePlot(double x, double y)
{
  if (plot_.size() >= n_) 
  {
    plot_.erase(plot_.begin());
  }
  plot_.push_back({x, y});
}

void Plot::draw()
{
  setLayer();
  eggx_moveto(win_, plot_.begin()->first, plot_.begin()->second );
  for (std::vector<std::pair<double, double> >::const_iterator it = plot_.begin(), e = plot_.end(); it != e; ++it) 
  {
    if(it->first < xmin_ || it->first > xmax_ || it->second < ymin_ || it->second > ymax_) 
      continue;
    eggx_drawcirc(win_, it->first, it->second, 0.1, 0.1 );
    eggx_line(win_, it->first, it->second, PENDOWN);
  }
  copylayer(win_,1,0) ;
  is_layerd_ = false;
}

void Plot::setLayer()
{
  if(!is_layerd_)
  {
    eggx_layer(win_, 0, 1);
    is_layerd_ = true;
  }
}
void Plot::clear()
{
  eggx_gclr(win_);
}

void Plot::drawAxis()
{
  setLayer();
  int fontsize = 10;
  eggx_window(win_, xmin_-fontsize*3, ymin_-fontsize, xmax_+fontsize, ymax_+fontsize);
  eggx_drawline( win_, xmin_, ymin_, xmin_, ymax_ ) ;
  eggx_drawline( win_, xmin_, ymax_, xmax_, ymax_ ) ;
  eggx_drawline( win_, xmax_, ymax_, xmax_, ymin_ ) ;
  eggx_drawline( win_, xmax_, ymin_, xmin_, ymin_ ) ;
  eggx_drawstr(win_, xmin_, ymin_-fontsize/4, fontsize, 0.0, "%f", xmin_);
  eggx_drawstr(win_, xmax_, ymin_-fontsize/4, fontsize, 0.0, "%f", xmax_);
  eggx_drawstr(win_, xmin_-fontsize/2, ymin_, fontsize, 0.0, "%f", ymin_);
  eggx_drawstr(win_, xmin_-fontsize/2, ymax_, fontsize, 0.0, "%f", ymax_);
}
void Plot::drawAutoAxisX()
{
  std::vector<double> x;
  for (std::vector<std::pair<double, double> >::const_iterator it = plot_.begin(), e = plot_.end(); it != e; ++it) 
  {
    x.push_back(it->first);
  }
  xmin_ = *std::min_element(x.begin(), x.end());
  xmax_ = *std::max_element(x.begin(), x.end());
  drawAxis();
  std::vector<double>().swap(x);
}
void Plot::drawAutoAxisY()
{
  std::vector<double> y;
  for (std::vector<std::pair<double, double> >::const_iterator it = plot_.begin(), e = plot_.end(); it != e; ++it) 
  {
    y.push_back(it->second);
  }
  ymin_ = *std::min_element(y.begin(), y.end());
  ymax_ = *std::max_element(y.begin(), y.end());
  drawAxis();
  std::vector<double>().swap(y);
}
void Plot::drawAutoAxis()
{
  std::vector<double> x;
  std::vector<double> y;
  for (std::vector<std::pair<double, double> >::const_iterator it = plot_.begin(), e = plot_.end(); it != e; ++it) 
  {
    x.push_back(it->first);
    y.push_back(it->second);
  }
  xmin_ = *std::min_element(x.begin(), x.end());
  xmax_ = *std::max_element(x.begin(), x.end());
  ymin_ = *std::min_element(y.begin(), y.end());
  ymax_ = *std::max_element(y.begin(), y.end());
  int fontsize = 10;
  eggx_window(win_, xmin_-fontsize*3, ymin_-fontsize, xmax_+fontsize, ymax_+fontsize);
  eggx_drawline( win_, xmin_, ymin_, xmin_, ymax_ ) ;
  eggx_drawline( win_, xmin_, ymax_, xmax_, ymax_ ) ;
  eggx_drawline( win_, xmax_, ymax_, xmax_, ymin_ ) ;
  eggx_drawline( win_, xmax_, ymin_, xmin_, ymin_ ) ;
  eggx_drawstr(win_, xmin_, ymin_-fontsize/4, fontsize, 0.0, "%f", xmin_);
  eggx_drawstr(win_, xmax_, ymin_-fontsize/4, fontsize, 0.0, "%f", xmax_);
  eggx_drawstr(win_, xmin_-fontsize/2, ymin_, fontsize, 0.0, "%f", ymin_);
  eggx_drawstr(win_, xmin_-fontsize/2, ymax_, fontsize, 0.0, "%f", ymax_);
  std::vector<double>().swap(x);
  std::vector<double>().swap(y);
}

} // namespace ntlab