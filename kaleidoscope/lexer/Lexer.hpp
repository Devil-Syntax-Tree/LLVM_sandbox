#pragma once

#include <memory>

#include "Token.hpp"

namespace kaleidoscope {
class Reader;
class Lexer {
public:
  Lexer(std::unique_ptr<Reader> reader);
  Token nextToken();

private:
  std::unique_ptr<Reader> reader;
  char lastCharacter;
};
} // namespace kaleidoscope
