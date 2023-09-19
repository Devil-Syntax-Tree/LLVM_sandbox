#include "lexer/Lexer.hpp"
#include "lexer/Reader.hpp"
#include "lexer/Token.hpp"
#include "parser/Parser.hpp"
#include "parser/Precedence.hpp"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/raw_ostream.h>
#include <memory>

int main() {
  llvm::outs() << "Hello LLVM!!!\n\n";

  Lexer lexer(
      std::make_unique<Reader>("kaleidoscope/kaleidoscope_example.txt"));

  Precedence precedence(0);
  Parser parser(std::make_unique<Lexer>(std::make_unique<Reader>(
                    "kaleidoscope/kaleidoscope_example.txt")),
                std::make_unique<Precedence>(precedence));

  Token tok({TokenType::TOK_INIT, ""});
  parser.getNextToken();
  while (tok.type != TokenType::TOK_EOF) {
    tok = lexer.getToken();
    // std::cout << "got " << tok.value << " as token" << std::endl;
    parser.parse();
  }

  return 0;
}