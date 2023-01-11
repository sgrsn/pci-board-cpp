#include "../expart/common/basic.h"
#include <lib/Diff/differentiator.hpp>

int main(void) 
{
  ntlab::Differentiator<double> diff;

  double i = 0;
  cur_hide();
  while( !kbhit_linux() ){
    i += 0.1;
    printf("Time: %8.4f[s]    (Hit ENTER to stop)\n", diff.update(i) );
    cur_up(1);
    usleep(1000);
  }
  cur_down(2);
  cur_show();

  return 0;
}
