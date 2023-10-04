#pragma once

#include <string>

namespace kaleidoscope {
struct Token {
  enum class TokenType {
    INIT,
    END_OF_FILE,
    ID,
    NUMBER,
    DEF,
    EXTERN,
    IF,
    THEN,
    ELSE,
    LEFT_PARENTHESES,
    RIGHT_PARENTHESES,
    ASSIGN,
    SUM,
    MINUS,
    MULT,
    DIV,
    LESS,
    GREATER,
    UNKNOWN
  };

  TokenType type;
  std::string value;
};

std::string to_string(Token::TokenType value);
} // namespace kaleidoscope
