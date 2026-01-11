#include <llvm/Passes/PassBuilder.h>
#include <llvm/Analysis/PostDominators.h>
#include <llvm/Analysis/ScalarEvolution.h>
#include <llvm/Analysis/BlockFrequencyInfo.h>
#include <llvm/Analysis/BranchProbabilityInfo.h>
#include <llvm/Analysis/AssumptionCache.h>

/**
 * Removing unused strings that are created by ZiYue4D.
 */

class RemoveUnusedStringPass : public llvm::PassInfoMixin<RemoveUnusedStringPass> {
public:
    llvm::PreservedAnalyses run(llvm::Function& function, llvm::FunctionAnalysisManager& manager) {
        std::vector<llvm::Instruction*> pending_remove{};

        for (llvm::BasicBlock& block : function) {
            for (llvm::Instruction& instruction : block) {
                const llvm::CallInst* call = llvm::dyn_cast<llvm::CallInst>(&instruction);
                if (call == nullptr) continue;

                if (call->getCalledFunction()->getName() == "ziyue4d_create_string__") {
                    bool should_remove = true;
                    for (const llvm::Use& use : call->uses()) {
                        if (const auto* invoke = llvm::dyn_cast<llvm::CallInst>(use.getUser())) {
                            if (invoke->getCalledFunction()->getName() != "ziyue4d_release_string__") {
                                should_remove = false;
                                break;
                            }
                        } else {
                            should_remove = false;
                            break;
                        }
                    }
                    if (should_remove) {
                        for (const llvm::Use& use : instruction.uses()) {
                            llvm::dyn_cast<llvm::CallInst>(use.getUser())->eraseFromParent();
                        }
                        pending_remove.push_back(&instruction);
                    }
                }
            }
        }

        for (llvm::Instruction* value : pending_remove) value->eraseFromParent();

        llvm::PreservedAnalyses analyses = llvm::PreservedAnalyses::none();
        analyses.preserve<llvm::DominatorTreeAnalysis>();
        analyses.preserve<llvm::PostDominatorTreeAnalysis>();
        analyses.preserve<llvm::LoopAnalysis>();
        analyses.preserve<llvm::BlockFrequencyAnalysis>();
        analyses.preserve<llvm::BranchProbabilityAnalysis>();
        analyses.preserve<llvm::ScalarEvolutionAnalysis>();
        analyses.preserve<llvm::AssumptionAnalysis>();
        return analyses;
    }
};
