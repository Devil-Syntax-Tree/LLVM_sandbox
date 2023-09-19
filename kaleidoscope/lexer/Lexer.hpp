#pragma once

#include <string>
#include <memory>
#include "lexer/Reader.hpp"
#include "lexer/Token.hpp"

class Lexer {
public:
    //Lexer(Reader &r);
    Lexer(std::unique_ptr<Reader> reader);
    ~Lexer() = default;
    std::string getIdentifierStr();
    double getValNum();
    Token getToken();

private:
    std::unique_ptr<Reader> reader;
    //int lastCharacter;
    std::string identifierStr;
    double valNum;
};
