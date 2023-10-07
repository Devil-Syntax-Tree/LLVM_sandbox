#include "../lexer/Lexer.hpp"
#include "../lexer/Token.hpp"
#include "../reader/Reader.hpp"

kaleidoscope::Lexer::Lexer(std::unique_ptr<Reader> r) : reader{std::move(r)} {}

std::string kaleidoscope::Lexer::getIdentifierStr() { return identifierStr; }

double kaleidoscope::Lexer::getValNum() { return valNum; }

kaleidoscope::Token kaleidoscope::Lexer::nextToken() {
  using TokenType = Token::TokenType;

  char currentCharacter;
  if (pendingCharacter.has_value()) {
    currentCharacter = pendingCharacter.value();
    pendingCharacter.reset();
  } else {
    currentCharacter = reader->nextCharacter();
  }

  // Skip spacing characters
  while (std::isspace(currentCharacter)) {
    currentCharacter = reader->nextCharacter();
  }

  std::string identifier;
  // [aA-zZ][aA-zZ0-9]*
  if (std::isalpha(currentCharacter)) {
    identifier += currentCharacter;
    while (std::isalnum(currentCharacter = reader->nextCharacter())) {
      identifier += currentCharacter;
    }

    pendingCharacter = currentCharacter;

    if (identifier == "def") {
      return {TokenType::DEF, identifier};
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

  // [0-9.]+
  if (isdigit(currentCharacter) || currentCharacter == '.') {
    std::string doubleRepresentation;
    do {
      doubleRepresentation += currentCharacter;
      currentCharacter = reader->nextCharacter();
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
  if (lastCharacter == ',') {
    return {TokenType::COMMA, std::string(1, lastCharacter)};
  }

  if (currentCharacter == EOF) {
    return {TokenType::END_OF_FILE, "EOF"};
  }

  // wildcard
  return {TokenType::UNKNOWN, std::string(1, currentCharacter)};
}
