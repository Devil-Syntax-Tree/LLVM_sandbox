#include <iostream>
#include <memory>

#include <llvm/ExecutionEngine/Orc/LLJIT.h>
#include <llvm/ExecutionEngine/Orc/ThreadSafeModule.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/InitLLVM.h>
#include <llvm/Support/TargetSelect.h>

int main(int argc, char **argv) {
  if (llvm::InitializeNativeTarget()) {
    std::cerr << "Unable to initialize native target\n";
  }

  if (llvm::InitializeNativeTargetAsmPrinter()) {
    std::cerr << "Unable to initialize native target ASM printer\n";
  }

  auto context{std::make_unique<llvm::LLVMContext>()};
  auto module{std::make_unique<llvm::Module>("root", *context)};

  llvm::orc::ThreadSafeModule threadSafeModule{std::move(module),
                                               std::move(context)};

  auto JIT{llvm::orc::LLJITBuilder{}.create()};

  if (auto E = JIT.takeError()) {
    std::cerr << "Problem with creating LLJIT. " << llvm::toString(std::move(E))
              << '\n';
  }

  (*JIT)->addIRModule(std::move(threadSafeModule));

  return 0;
}
