#include <exception>

#include "FileReader.hpp"
namespace kaleidoscope {
FileReader::FileReader(const std::string &path) : file{path} {
  if (!file.is_open()) {
    throw std::runtime_error("Could not open file...");
  }
}

char FileReader::nextCharacter() { return file.get(); }
} // namespace kaleidoscope
