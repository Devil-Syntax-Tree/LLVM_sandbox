#include "HelloWorld.hpp"

#include "llvm/Passes/PassBuilder.h"
#include "llvm/Support/raw_ostream.h"

llvm::PreservedAnalyses
PassExamples::HelloWorldPass::run(llvm::Function &F,
                                  llvm::FunctionAnalysisManager &AM) {
  llvm::errs() << F.getName() << '\n';
  return llvm::PreservedAnalyses::all();
}

llvm::PassPluginLibraryInfo getHelloWorldPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "hello-world", LLVM_VERSION_STRING,
          [](llvm::PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](llvm::StringRef Name, llvm::FunctionPassManager &FPM,
                   llvm::ArrayRef<llvm::PassBuilder::PipelineElement>) {
                  if (Name == "hello-world") {
                    FPM.addPass(PassExamples::HelloWorldPass());
                    return true;
                  }
                  return false;
                });
          }};
}
