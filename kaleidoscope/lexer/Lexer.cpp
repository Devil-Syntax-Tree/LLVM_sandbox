#include "Lexer.hpp"
#include "Token.hpp"

#include "../reader/Reader.hpp"

kaleidoscope::Lexer::Lexer(std::unique_ptr<Reader> r) : reader{std::move(r)} {}

kaleidoscope::Token kaleidoscope::Lexer::nextToken() {
  using TokenType = Token::TokenType;

  char currentCharacter;
  if (pendingCharacter.has_value()) {
    currentCharacter = pendingCharacter.value();
    pendingCharacter.reset();
  } else {
    currentCharacter = reader->nextChar();
  }

  // Skip spacing characters
  while (std::isspace(currentCharacter)) {
    currentCharacter = reader->nextChar();
  }

  std::string identifier;
  // [aA-zZ][aA-zZ0-9]*
  if (std::isalpha(currentCharacter)) {
    identifier += currentCharacter;
    while (std::isalnum(currentCharacter = reader->nextChar())) {
      identifier += currentCharacter;
    }

    pendingCharacter = currentCharacter;

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
  if (isdigit(currentCharacter) || currentCharacter == '.') {
    std::string doubleRepresentation;
    do {
      doubleRepresentation += currentCharacter;
      currentCharacter = reader->nextChar();
    } while (isdigit(currentCharacter) || currentCharacter == '.');

    pendingCharacter = currentCharacter;

    auto literalValue = std::stod(doubleRepresentation);
    return {TokenType::NUMBER, "DOUBLE_NUMBER", literalValue};
  }

  std::string characterRepresentation(1, currentCharacter);

  if (currentCharacter == '=') {
    return {TokenType::ASSIGN, characterRepresentation};
  }
  if (currentCharacter == '+') {
    return {TokenType::SUM, characterRepresentation};
  }
  if (currentCharacter == '-') {
    return {TokenType::MINUS, characterRepresentation};
  }
  if (currentCharacter == '*') {
    return {TokenType::MULT, characterRepresentation};
  }
  if (currentCharacter == '/') {
    return {TokenType::DIV, characterRepresentation};
  }
  if (currentCharacter == '<') {
    return {TokenType::LESS, characterRepresentation};
  }
  if (currentCharacter == '>') {
    return {TokenType::GREATER, characterRepresentation};
  }
  if (currentCharacter == '(') {
    return {TokenType::LEFT_PARENTHESES, characterRepresentation};
  }
  if (currentCharacter == ')') {
    return {TokenType::RIGHT_PARENTHESES, characterRepresentation};
  }

  if (currentCharacter == EOF) {
    return {TokenType::END_OF_FILE, "EOF"};
  }

  // wildcard
  return {TokenType::UNKNOWN, std::string(1, currentCharacter)};
}
