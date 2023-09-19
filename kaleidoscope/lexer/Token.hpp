#pragma once

#include <iostream>
#include <fstream>
#include <string>

enum class TokenType {
    TOK_INIT,
    TOK_EOF,
    TOK_ID,
    TOK_NUMBER,
    TOK_DEF,
    TOK_EXTERN,
    TOK_IF,
    TOK_THEN,
    TOK_ELSE,
    TOK_L_PAR,
    TOK_R_PAR,
    TOK_ASSIGN,
    TOK_SUM,
    TOK_DIFF,
    TOK_MULT,
    TOK_DIV,
    TOK_LESS,
    TOK_GREATER,
    TOK_COMMA,
    TOK_DOT,
    TOK_UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;
};
