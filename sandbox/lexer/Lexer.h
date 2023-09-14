#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <fstream>
#include <string>
#include <lexer/Reader.h>

class Lexer {
public:
    Lexer(Reader &r);
    ~Lexer();
    int getToken();

private:
    Reader& reader;
    int lastCharacter;
    std::string identifierStr;
    double valNum;
};

#endif // LEXER_H