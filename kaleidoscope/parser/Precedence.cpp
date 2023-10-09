#include "../lexer/Token.hpp"
#include "Precedence.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <bits/stdc++.h>

kaleidoscope::Precedence::Precedence(int n) : num(n) {
    precedenceMap['<'] = 1;
    precedenceMap['>'] = 2;
    precedenceMap['+'] = 3;
    precedenceMap['-'] = 4;
    precedenceMap['*'] = 5;
    precedenceMap['/'] = 6; // más grande
}

int kaleidoscope::Precedence::getTokenPrecedence(Token curTok) {
    char arr[curTok.value.length() + 1];
    strcpy(arr, curTok.value.c_str());
    char curTokInt = arr[0];

    if(!isascii(curTokInt)) {
        return -1;
    }

    int tokPrec = precedenceMap[curTokInt];
    if (tokPrec <= 0) {
        return -1;
    }

    return tokPrec;
}

