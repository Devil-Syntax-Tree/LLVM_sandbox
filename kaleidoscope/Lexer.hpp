#pragma once

class std::string;
class Reader;

class Lexer {
public:
  Lexer(Reader &r);
  Token getNextToken();

private:
  Reader &reader; // NOTE: this is very bad!
  int lastCharacter;
  std::string identifierStr;
  double valNum;
};
