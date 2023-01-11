#include "log.hpp"

namespace ntlab
{
Log::Log(const char *filename) : ofs_(filename)
{
  if (!ofs_)
  {
    std::cout << "can not open the file" << std::endl;
  }
}
void Log::writeLegend(std::vector<std::string> legends)
{
  for (std::vector<std::string>::const_iterator it = legends.begin(), e = legends.end(); it != e; ++it)
  {
    ofs_ << *it << ", ";
  }
  ofs_ << std::endl;
}
void Log::writeData(std::vector<double> data)
{
  for (std::vector<double>::const_iterator it = data.begin(), e = data.end(); it != e; ++it)
  {
    ofs_ << *it << ", ";
  }
  ofs_ << std::endl;
}

}// namespace ntlab