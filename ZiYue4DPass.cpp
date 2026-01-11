#include "RemoveUnusedStringPass.cpp"
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>

/**
 * ZiYue4D Pass Collection.
 */

extern "C" LLVM_ATTRIBUTE_WEAK llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
    return {
        LLVM_PLUGIN_API_VERSION,
        "ZiYue4D Pass Collection",
        "0.0.1",
        [](llvm::PassBuilder& PB) {
            PB.registerPipelineParsingCallback(
                [](const llvm::StringRef Name, llvm::FunctionPassManager& FPM,
                   llvm::ArrayRef<llvm::PassBuilder::PipelineElement>) -> bool {
                    if (Name == "remove-unused-string") {
                        FPM.addPass(RemoveUnusedStringPass());
                        return true;
                    }
                    return false;
                });
        }
    };
}
