#pragma once

#include <iostream>
#include <fstream>
#include <string>

class Reader {
public:
    Reader(const std::string& filename);
    ~Reader();
    void readFile();
    char readChar();
    char peekNextChar();

private:
    std::string filename_;
    std::ifstream file_;
};