#include "Lexer.hpp"
#include "Token.hpp"

#include "../reader/Reader.hpp"

kaleidoscope::Lexer::Lexer(std::unique_ptr<Reader> r) : reader{std::move(r)} {}

kaleidoscope::Token kaleidoscope::Lexer::nextToken() {
  using TokenType = Token::TokenType;
  // Initial read
  lastCharacter = reader->nextCharacter();

  // Skip spacing characters
  while (std::isspace(lastCharacter)) {
    lastCharacter = reader->nextCharacter();
  }

  std::string identifier;
  // [aA-zZ][aA-zZ0-9]*
  if (std::isalpha(lastCharacter)) {
    identifier += lastCharacter;
    while (std::isalnum(lastCharacter = reader->nextCharacter())) {
      identifier += lastCharacter;
    }

    if (identifier == "def") {
      return {TokenType::DEF, identifier};
    }
    if (identifier == "extern") {
      return {TokenType::EXTERN, identifier};
    }
    if (identifier == "if") {
      return {TokenType::IF, identifier};
    }
    if (identifier == "then") {
      return {TokenType::THEN, identifier};
    }
    if (identifier == "else") {
      return {TokenType::ELSE, identifier};
    }

    return {TokenType::ID, identifier};
  }

  // [0-9.]+
  if (isdigit(lastCharacter) || lastCharacter == '.') {
    std::string doubleRepresentation;
    do {
      doubleRepresentation += lastCharacter;
      lastCharacter = reader->nextCharacter();
    } while (isdigit(lastCharacter) || lastCharacter == '.');

    auto literalValue = std::stod(doubleRepresentation);
    return {TokenType::NUMBER, "DOUBLE_NUMBER", literalValue};
  }

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

  if (lastCharacter == EOF) {
    return {TokenType::END_OF_FILE, "EOF"};
  }

  // wildcard
  return {TokenType::UNKNOWN, std::string(1, lastCharacter)};
}
