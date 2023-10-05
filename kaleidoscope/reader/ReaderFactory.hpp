#pragma once

#include "Reader.hpp"
#include <memory>

namespace kaleidoscope {
class ReaderFactory {
public:
  static std::unique_ptr<Reader> createReader(const std::string& filePath);
  static std::unique_ptr<Reader> createReader();

};
} // namespace kaleidoscope
