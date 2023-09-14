#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <fstream>
#include <string>

// enum Token { // toca usar números o chars... :(
    // TOK_EOF = -1,
    // TOK_ID = -2,
    // TOK_NUMBER = -3,
    // TOK_DEF = -4,
    // TOK_EXTERN = -5,
    // TOK_IF = -6,
    // TOK_THEN = -7,
    // TOK_ELSE = -8,
    // TOK_L_PAR = -9,
    // TOK_R_PAR = -10,
    // TOK_ASSIGN = -11,
    // TOK_SUM = -12,
    // TOK_DIFF = -13,
    // TOK_MULT = -14,
    // TOK_DIV = -15,
    // TOK_LESS = -16,
    // TOK_GREATER = -17
// };

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
    TOK_UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;
};

#endif // TOKEN_H