#pragma once

#include "../reader/Reader.hpp"
#include "Token.hpp"
#include <memory>
#include <optional>


namespace kaleidoscope {

class Lexer {
public:
  Lexer(std::unique_ptr<Reader> reader);
  std::string getIdentifierStr();
  double getValNum();
  Token nextToken();
  char getCurrentCharacter();

private:
  void skipSpaces();
  std::optional<Token> checkIdentifiers();
  std::optional <Token> checkNumber();
  std::optional <Token> checkSymbols();

  std::unique_ptr<Reader> reader;
  char currentCharacter;
  std::string identifierStr;
  double valNum;
  std::optional<char> pendingCharacter;
};

} // namespace kaleidoscope
