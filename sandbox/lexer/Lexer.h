#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <fstream>
#include <string>
#include <lexer/Token.h>

class Lexer {
public:
    Lexer(std::function<char()> r);
    ~Lexer();
    int getToken();

private:
    std::function<int()> reader;
    int lastCharacter;
    std::string identifierStr;
    double valNum;
};

#endif // LEXER_H