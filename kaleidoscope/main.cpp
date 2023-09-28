#include "codegen/Codegen.hpp"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/raw_ostream.h>
#include <memory>

int main() {
  llvm::outs() << "Hello LLVM!!!\n\n";
  
  // ¿¡alguien sabe mejorar el manejo de memoria acá?!
  // Lexer lexer(
  //     std::make_unique<Reader>("kaleidoscope/kaleidoscope.txt"));

  // Precedence precedence(0);
  // Parser parser(std::make_unique<Lexer>(std::make_unique<Reader>(
  //                   "kaleidoscope/kaleidoscope.txt")),
  //               std::make_unique<Precedence>(precedence));

  // Token tok({TokenType::TOK_INIT, ""});
  // parser.getNextToken();
  // while (tok.type != TokenType::TOK_EOF) {
  //   tok = lexer.getToken();
  //   parser.parse();
  // }

  // std::cout << "Success!" << std::endl;

  Codegen codegen;

  codegen.initializeStaticMembers();
  codegen.initializeModule();
  codegen.mainLoop();
  //Codegen::TheModule->print(errs(), nullptr);

  return 0;
}