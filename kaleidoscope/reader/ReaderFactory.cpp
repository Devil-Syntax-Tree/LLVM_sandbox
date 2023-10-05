#include "ReaderFactory.hpp"
#include "ConsoleReader.hpp"
#include "FileReader.hpp"
#include <memory>
#include <string>

std::unique_ptr<kaleidoscope::Reader>
kaleidoscope::ReaderFactory::createReader(const std::string& filePath) {
  return std::make_unique<kaleidoscope::FileReader>(filePath);
}

std::unique_ptr<kaleidoscope::Reader>
kaleidoscope::ReaderFactory::createReader() {
  return std::make_unique<kaleidoscope::ConsoleReader>();
}
