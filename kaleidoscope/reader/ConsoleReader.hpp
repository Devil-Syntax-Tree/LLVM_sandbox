#pragma once

#include "Reader.hpp"

namespace kaleidoscope {
class ConsoleReader : public Reader {
public:
  char nextChar();
};
} // namespace kaleidoscope
