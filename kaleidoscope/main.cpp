#include <string>

#include "Lexer.hpp"

int main(int argc, char **argv) {
  std::string test{"1.3231"};

  Lexer lexer {
    []() {}
  }

  return 0;
}
