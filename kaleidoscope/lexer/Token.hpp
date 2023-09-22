#pragma once

#include <iostream>
#include <fstream>
#include <string>

enum class TokenType {
    
    TOK_INIT = -1,
    TOK_EOF = -2,
    TOK_ID = -3,
    TOK_NUMBER = -4,
    TOK_DEF = -5,
    TOK_EXTERN = -6,
    TOK_IF = -7,
    TOK_THEN = -8,
    TOK_ELSE = -9,
    TOK_L_PAR = -10,
    TOK_R_PAR = -11,
    TOK_ASSIGN = -12,
    TOK_SUM = -13,
    TOK_DIFF = -14,
    TOK_MULT = -15,
    TOK_DIV = -16,
    TOK_LESS = -17,
    TOK_GREATER = -18,
    TOK_COMMA = -19,
    TOK_DOT = -20,
    TOK_UNKNOWN = -21
};

struct Token {
    TokenType type;
    std::string value;
};
