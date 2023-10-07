#pragma once

#include "Reader.hpp"

namespace kaleidoscope {
class ConsoleReader : public Reader {
public:
  char nextCharacter() override;
};
} // namespace kaleidoscope
