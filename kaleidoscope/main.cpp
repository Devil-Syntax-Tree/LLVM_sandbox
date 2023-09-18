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

    Reader reader("kaleidoscope/kaleidoscope_example.txt");

    //Lexer lexer([&reader]() { return reader.readChar(); });
    Lexer lexer(reader);

    Token tok({ TokenType::TOK_INIT, "" });
    while (tok.type != TokenType::TOK_EOF) {
        tok = lexer.getToken();
        std::cout << "got " << tok.value << " as token" << std::endl;
    }

    return 0;
}