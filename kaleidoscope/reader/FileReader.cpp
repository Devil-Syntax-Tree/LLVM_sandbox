#include <exception>

#include "FileReader.hpp"

kaleidoscope::FileReader::FileReader(std::filesystem::path path) : file{path} {
  if (!file.is_open()) {
    throw std::runtime_error("Could not open file...");
  }
}

char kaleidoscope::FileReader::nextCharacter() { return file.get(); }
