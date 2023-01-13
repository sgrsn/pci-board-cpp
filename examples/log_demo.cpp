#include <iostream>
#include <fstream>
#include <lib/log/log.hpp>
#include <vector>
#include <string>
#include "../expart/common/basic.h"

int main() {
  // Log file
  char outstr[30];
  time_t t = time(NULL);
  strftime(outstr, sizeof(outstr), "log/logdemo%Y%m%d%H%M%S.csv", localtime(&t));
  ntlab::Log log(outstr);
  std::vector<std::string> legends;
  legends.push_back("x");
  legends.push_back("sin");
  legends.push_back("cos");
  legends.push_back("tan");
  log.writeLegend(legends);
  std::vector<std::string>().swap(legends);

  double x = 0;

  while( !kbhit_linux() )
  {
    x += 0.01;
    // Dump log data
    std::vector<double> log_data;
    log_data.push_back(x);
    log_data.push_back(sin(x));
    log_data.push_back(cos(x));
    log_data.push_back(tan(x));
    log.writeData(log_data);
    std::vector<double>().swap(log_data);
    printf("%8.3f\n", x);
    cur_up(1);
  }
    
  return 0;
}