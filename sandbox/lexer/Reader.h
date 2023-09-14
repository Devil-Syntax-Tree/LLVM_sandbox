#ifndef READER_H
#define READER_H

#include <iostream>
#include <fstream>
#include <string>

class Reader {
public:
    Reader(const std::string& filename);
    ~Reader();
    void readFile();
    char readChar();

private:
    std::string filename_;
    std::ifstream file_;
};

#endif // READER_H