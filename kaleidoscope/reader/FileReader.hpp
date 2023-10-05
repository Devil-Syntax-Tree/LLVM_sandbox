#pragma once

#include "Reader.hpp"
#include <string>
#include <fstream>

namespace kaleidoscope {
class FileReader : public Reader {
public:
  FileReader(const std::string& filename);
  ~FileReader();
  void readFile();
  char nextChar() override;
  char peekNextChar() override;

private:
  std::string filename_;
  std::ifstream file_;
};
} // namespace kaleidoscope
