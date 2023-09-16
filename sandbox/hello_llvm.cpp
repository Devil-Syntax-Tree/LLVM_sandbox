#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/raw_ostream.h>
#include <lexer/Reader.h>
#include <lexer/Lexer.h>
#include <lexer/Token.h>

int main() {
  llvm::outs() << "Hello LLVM!!!\n\n";

  llvm::LLVMContext context;
  llvm::IRBuilder<> irBuilder(context);
  llvm::Module module("root", context);

  auto i32 = irBuilder.getInt32Ty();
  auto prototype = llvm::FunctionType::get(i32, false);
  auto main_fn = llvm::Function::Create(
      prototype, llvm::Function::ExternalLinkage, "main", module);
  auto body = llvm::BasicBlock::Create(context, "body", main_fn);
  irBuilder.SetInsertPoint(body);

  module.print(llvm::outs(), nullptr);

  return 0;
}