#pragma once

#include "Reader.hpp"

namespace kaleidoscope {
class ConsoleReader : public Reader {
public:
  ~ConsoleReader() override {}
  char nextCharacter() override;
};
} // namespace kaleidoscope
