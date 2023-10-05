#include "../lexer/Lexer.hpp"
#include "../lexer/Token.hpp"
#include "../reader/Reader.hpp"

kaleidoscope::Lexer::Lexer(std::unique_ptr<Reader> r) : reader{std::move(r)} {}

std::string kaleidoscope::Lexer::getIdentifierStr() { return identifierStr; }

double kaleidoscope::Lexer::getValNum() { return valNum; }

kaleidoscope::Token kaleidoscope::Lexer::nextToken() {
  using TokenType = Token::TokenType;
  int lastCharacter = ' ';

  while (isspace(lastCharacter)) { // ignora espacios
    lastCharacter = reader->nextChar();
  }

  // reconocer identificadores -> [aA-zZ][aA-zZ0-9]*
  if (isalpha(lastCharacter)) { // isalpha() revisa si el primer carácter es
                                // letra (min o may)
    identifierStr = lastCharacter;
    while (isalnum(reader->peekNextChar())) { // isalnum() revisa que luego haya
                                              // letras o números
      lastCharacter = reader->nextChar();
      identifierStr += lastCharacter;
    }

    if (identifierStr == "def") {
      return {TokenType::DEF, identifierStr};
    }
    if (identifierStr == "extern") {
      return {TokenType::EXTERN, identifierStr};
    }
    if (identifierStr == "if") {
      return {TokenType::IF, identifierStr};
    }
    if (identifierStr == "then") {
      return {TokenType::THEN, identifierStr};
    }
    if (identifierStr == "else") {
      return {TokenType::ELSE, identifierStr};
    }

    return {TokenType::ID, identifierStr};
  }

  // reconocer números -> [0-9.]+
  if (isdigit(lastCharacter) || lastCharacter == '.') {
    std::string numStr;
    numStr += lastCharacter;
    while (isdigit(reader->peekNextChar()) || reader->peekNextChar() == '.') {
      lastCharacter = reader->nextChar();
      numStr += lastCharacter;
    }

    valNum = strtod(numStr.c_str(), 0);
    return {TokenType::NUMBER, numStr};
  }

  // reconocer símbolos
  if (lastCharacter == '=') {
    return {TokenType::ASSIGN, std::string(1, lastCharacter)};
  }
  if (lastCharacter == '+') {
    return {TokenType::SUM, std::string(1, lastCharacter)};
  }
  if (lastCharacter == '-') {
    return {TokenType::MINUS, std::string(1, lastCharacter)};
  }
  if (lastCharacter == '*') {
    return {TokenType::MULT, std::string(1, lastCharacter)};
  }
  if (lastCharacter == '/') {
    return {TokenType::DIV, std::string(1, lastCharacter)};
  }
  if (lastCharacter == '<') {
    return {TokenType::LESS, std::string(1, lastCharacter)};
  }
  if (lastCharacter == '>') {
    return {TokenType::GREATER, std::string(1, lastCharacter)};
  }
  if (lastCharacter == '(') {
    return {TokenType::LEFT_PARENTHESES, std::string(1, lastCharacter)};
  }
  if (lastCharacter == ')') {
    return {TokenType::RIGHT_PARENTHESES, std::string(1, lastCharacter)};
  }
  if (lastCharacter == ',') {
    return {TokenType::COMMA, std::string(1, lastCharacter)};
  }

  // reconocer EOF
  if (lastCharacter == EOF) {
    return {TokenType::END_OF_FILE, "EOF"};
  }

  // caso para símbolos extraños... realmente, esto no debería existir...
  // en todo caso, retorna su código ASCII
  // int thisCharacter = lastCharacter;
  lastCharacter = reader->nextChar();
  return {TokenType::UNKNOWN, std::string(1, lastCharacter)};
}
