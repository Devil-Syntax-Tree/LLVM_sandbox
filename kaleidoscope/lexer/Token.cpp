#include "Token.hpp"

std::string kaleidoscope::to_string(Token::TokenType value) {
  using namespace std::string_literals;
  using TokenType = Token::TokenType;

  switch (value) {
  case TokenType::INIT:
    return "INIT"s;
  case TokenType::END_OF_FILE:
    return "EOF"s;
  case TokenType::ID:
    return "ID"s;
  case TokenType::NUMBER:
    return "NUMBER"s;
  case TokenType::DEF:
    return "DEF"s;
  case TokenType::EXTERN:
    return "EXTERN"s;
  case TokenType::IF:
    return "IF"s;
  case TokenType::THEN:
    return "THEN"s;
  case TokenType::ELSE:
    return "ELSE"s;
  case TokenType::LEFT_PARENTHESES:
    return "LEFT_PARENTHESES"s;
  case TokenType::RIGHT_PARENTHESES:
    return "RIGHT_PARENTHESES"s;
  case TokenType::ASSIGN:
    return "ASSIGN"s;
  case TokenType::SUM:
    return "SUM"s;
  case TokenType::MINUS:
    return "MINUS"s;
  case TokenType::MULT:
    return "MULT"s;
  case TokenType::DIV:
    return "DIV"s;
  case TokenType::LESS:
    return "LESS"s;
  case TokenType::GREATER:
    return "GREATER"s;
  default:
    return "UNKNOWN"s;
  }
}
