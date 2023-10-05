#pragma once

#include "Reader.hpp"

namespace kaleidoscope {
class ConsoleReader : public Reader {
public:
  char nextChar() override;
  char peekNextChar() override;

private:
  char buffer;
  bool hasBufferedChar = false;
};
} // namespace kaleidoscope
