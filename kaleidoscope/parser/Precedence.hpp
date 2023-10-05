#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <map>
#include "../lexer/Token.hpp"

namespace kaleidoscope {
class Precedence {
public:
    Precedence(int n);
    ~Precedence() = default;
    int getTokenPrecedence(Token curTok);

private:
    std::map<char, int> precedenceMap;
    int num;
};

} // namespace kaleidoscope