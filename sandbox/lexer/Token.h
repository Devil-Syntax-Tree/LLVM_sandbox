#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <fstream>
#include <string>

enum Token { // toca usar números o chars... :(
    TOK_EOF = -1,
    TOK_ID = -2,
    TOK_NUMBER = -3
};

#endif // TOKEN_H