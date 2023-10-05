#pragma once

#include <memory>

#include "../lexer/Token.hpp"
#include "../reader/Reader.hpp"

namespace kaleidoscope {
class Reader;
class Lexer {
public:
  Lexer(std::unique_ptr<Reader> reader);
  std::string getIdentifierStr();
  double getValNum();
  Token nextToken();

private:
  std::unique_ptr<Reader> reader;
  char lastCharacter;
  std::string identifierStr;
  double valNum;
};

} // namespace kaleidoscope
