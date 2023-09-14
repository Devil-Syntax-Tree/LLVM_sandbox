#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <fstream>
#include <string>
#include <lexer/Reader.h>
#include <lexer/Token.h>

class Lexer {
public:
    Lexer(Reader &r);
    ~Lexer() = default;
    Token getToken();

private:
    Reader& reader;
    int lastCharacter;
    std::string identifierStr;
    double valNum;
};

#endif // LEXER_H