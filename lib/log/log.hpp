#ifndef LIB_NTLAB_LOG_HPP
#define LIB_NTLAB_LOG_HPP

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

namespace ntlab
{

class Log
{
 public:
  Log(const char *filename);
  void writeLegend(std::vector<std::string> legends);
  void writeData(std::vector<double> data);

 private:
  std::ofstream ofs_;

};  // Log

} // ntlab

  
#endif // LIB_NTLAB_LOG_HPP