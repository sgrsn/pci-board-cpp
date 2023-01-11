#include <stdio.h>
#include <lib/eggx_plot/plot.hpp>
#include "../expart/common/basic.h"
#include <math.h>

int main(void)
{
  ntlab::Plot plot(1000);
  plot.setXLim(0, 100);
  plot.setYLim(-1.1, 1.1);

  double x = 0;
  do{
    x += 0.1;
    plot.clear();
    plot.updatePlot(x, sin(x)*10);
    plot.drawAutoAxis();
    plot.draw();
	}while( !kbhit_linux() );

  plot.close();
	return(0);
}
