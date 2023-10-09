#pragma once

#include <memory>
#include <optional>

#include "Token.hpp"

namespace kaleidoscope {
class Reader;
class Lexer {
public:
  Lexer(std::unique_ptr<Reader> reader);
  std::string getIdentifierStr();
  double getValNum();
  Token nextToken();

private:
  void skipSpaces();
  std::optional<Token> checkIdentifiers();
  std::optional <Token> checkNumber();
  std::optional <Token> checkSymbols();

  std::unique_ptr<Reader> reader;
  char currentCharacter;
  std::optional<char> pendingCharacter;
};

} // namespace kaleidoscope
