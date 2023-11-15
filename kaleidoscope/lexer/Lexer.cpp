#include "Lexer.hpp"
#include "../reader/Reader.hpp"

namespace kaleidoscope {

using TokenType = Token::TokenType;

Lexer::Lexer(std::unique_ptr<Reader> r) : reader{std::move(r)} {}

std::string Lexer::getIdentifierStr() { return identifierStr; }

double Lexer::getValNum() { return valNum; }

Token Lexer::nextToken() {
  if (pendingCharacter.has_value()) {
    currentCharacter = pendingCharacter.value();
    pendingCharacter.reset();
  } else {
    currentCharacter = reader->nextCharacter();
  }

  skipSpaces();

  auto ckIdentifier{checkIdentifiers()};
  if (ckIdentifier.has_value()) {
    return ckIdentifier.value();
  }

  auto ckNumber{checkNumber()};
  if (ckNumber.has_value()) {
    return ckNumber.value();
  }

  auto ckSymbols{checkSymbols()};
  if (ckSymbols.has_value()) {
    return ckSymbols.value();
  }

  std::string characterRepresentation(1, currentCharacter);

  // wildcard
  return {TokenType::UNKNOWN, characterRepresentation};
}

void Lexer::skipSpaces() {
  while (std::isspace(currentCharacter)) {
    currentCharacter = reader->nextCharacter();
  }
}

std::optional<Token> kaleidoscope::Lexer::checkIdentifiers() {
  std::string identifier;
  // [aA-zZ][aA-zZ0-9]*
  if (std::isalpha(currentCharacter)) {
    identifier += currentCharacter;
    while (std::isalnum(currentCharacter = reader->nextCharacter())) {
      identifier += currentCharacter;
    }

    pendingCharacter = currentCharacter;

    if (identifier == "def") {
      return {{TokenType::DEF, identifier}};
    }
    if (identifier == "extern") {
      return {{TokenType::EXTERN, identifier}};
    }
    if (identifier == "if") {
      return {{TokenType::IF, identifier}};
    }
    if (identifier == "then") {
      return {{TokenType::THEN, identifier}};
    }
    if (identifier == "else") {
      return {{TokenType::ELSE, identifier}};
    }

    identifierStr = identifier;
    return {{TokenType::ID, identifier}};
  }

  return {};
}

std::optional<Token> Lexer::checkNumber() {
  // [0-9.]+
  if (isdigit(currentCharacter) || currentCharacter == '.') {
    std::string doubleRepresentation;
    do {
      doubleRepresentation += currentCharacter;
      currentCharacter = reader->nextCharacter();
    } while (isdigit(currentCharacter) || currentCharacter == '.');

    pendingCharacter = currentCharacter;

    auto literalValue = std::stod(doubleRepresentation);
    valNum = literalValue;
    return {{TokenType::NUMBER, "DOUBLE_NUMBER", literalValue}};
  }
  return {};
}

std::optional<Token> kaleidoscope::Lexer::checkSymbols() {
  std::string characterRepresentation(1, currentCharacter);

  if (currentCharacter == '=') {
    return {{TokenType::ASSIGN, characterRepresentation}};
  }
  if (currentCharacter == '+') {
    return {{TokenType::SUM, characterRepresentation}};
  }
  if (currentCharacter == '-') {
    return {{TokenType::MINUS, characterRepresentation}};
  }
  if (currentCharacter == '*') {
    return {{TokenType::MULT, characterRepresentation}};
  }
  if (currentCharacter == '/') {
    return {{TokenType::DIV, characterRepresentation}};
  }
  if (currentCharacter == '<') {
    return {{TokenType::LESS, characterRepresentation}};
  }
  if (currentCharacter == '>') {
    return {{TokenType::GREATER, characterRepresentation}};
  }
  if (currentCharacter == '(') {
    return {{TokenType::LEFT_PARENTHESES, characterRepresentation}};
  }
  if (currentCharacter == ')') {
    return {{TokenType::RIGHT_PARENTHESES, characterRepresentation}};
  }
  if (currentCharacter == ';') {
    return {{TokenType::SEMICOLON, characterRepresentation}};
  }
  if (currentCharacter == ',') {
    return {{TokenType::COMMA, characterRepresentation}};
  }

  if (currentCharacter == EOF) {
    return {{TokenType::END_OF_FILE, "EOF"}};
  }

  return {};
}
} // namespace kaleidoscope
