#include <lib/log/log.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>

int main(void) 
{ 
  char outstr[30];
  time_t t = time(NULL);
  strftime(outstr, sizeof(outstr), "log/log%Y%m%d%H%M%S.csv", localtime(&t));
  printf("%s\r\n", outstr);
  ntlab::Log log(outstr);
  std::vector<std::string> legends;
  std::vector<double> data;
  legends.push_back("data1");
  legends.push_back("data2");
  legends.push_back("data3");
  data.push_back(10);
  data.push_back(22);
  data.push_back(30);
  log.writeLegend(legends);
  log.writeData(data);

  return 0;
}
