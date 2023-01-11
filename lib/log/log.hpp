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
  Log(const char *filename) : ofs_(filename)
  {
    if (!ofs_)
    {
      std::cout << "can not open the file" << std::endl;
    }
  }
  void writeLegend(std::vector<string> legends)
  {
    for(auto e : legends)
    {
      ofs_ << e << ", ";
    }
    ofs_ << endl;
  }
  void writeData(std::vector<double> data)
  {
    for(auto e : data)
    {
      ofs_ << e << ", ";
    }
    ofs_ << endl;
  }

 private:
  std::ofstream ofs_;

};  // Log

} // ntlab

  
#endif // LIB_NTLAB_LOG_HPP