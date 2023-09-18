#include <memory>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/raw_ostream.h>
#include "lexer/Reader.hpp"
#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"

int main()
{
    llvm::outs() << "Hello LLVM!!!\n\n";

    Lexer lexer(std::make_unique<Reader>("kaleidoscope/kaleidoscope_example.txt"));

    Token tok({ TokenType::TOK_INIT, "" });
    while (tok.type != TokenType::TOK_EOF) {
        tok = lexer.getToken();
        std::cout << "got " << tok.value << " as token" << std::endl;
    }

    return 0;
}