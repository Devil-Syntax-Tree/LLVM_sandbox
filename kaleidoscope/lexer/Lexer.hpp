#pragma once

#include <string>
#include "lexer/Reader.hpp"
#include "lexer/Token.hpp"

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
