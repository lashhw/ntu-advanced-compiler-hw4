#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {

void visitor(Function &F) {
    errs() << "digraph dataflow {\n";
    errs() << "  rankdir=LR;\n";
    errs() << "  { rank=same; input_group; }\n";
    errs() << "  { rank=same; output_group; }\n";
    errs() << "  subgraph cluster_inputs {\n";
    errs() << "    input_group [style=invis];\n";

    DenseMap<Value*, std::string> nodeNames;
    DenseMap<Value*, std::string> outNames;
    std::vector<std::pair<std::string, std::string>> outNamesList;

    unsigned inCounter = 0;
    unsigned outCounter = 0;
    for (auto &arg : F.args()) {
        if (!arg.getType()->isPointerTy()) {
            std::string inName = "in" + std::to_string(inCounter++);
            nodeNames[&arg] = inName;
            errs() << "    " << inName << " [label=\"" << arg.getName() << "\", shape=box];\n";
        } else {
            std::string outName = "out" + std::to_string(outCounter++);
            outNames[&arg] = outName;
            outNamesList.emplace_back(std::string(arg.getName()), outName);
        }
    }
    errs() << "  }\n";
  
    errs() << "  subgraph cluster_outputs {\n";
    errs() << "    output_group [style=invis];\n";
    for (const auto &outName : outNamesList)
        errs() << "    " << outName.second << " [label=\"" << outName.first << "\", shape=box];\n";
    errs() << "  }\n";

    unsigned opCounter = 0;
    for (auto &BB : F) {
        for (auto &I : BB) {
            if (isa<BinaryOperator>(I)) {
                std::string nodeName = "op" + std::to_string(opCounter++);
                nodeNames[&I] = nodeName;
              
                std::string opSymbol;
                switch (I.getOpcode()) {
                    case Instruction::FAdd: opSymbol = "+"; break;
                    case Instruction::FSub: opSymbol = "-"; break;
                    case Instruction::FMul: opSymbol = "*"; break;
                    case Instruction::FDiv: opSymbol = "/"; break;
                    default: assert(false && "Unsupported operation");
                }

                errs() << "  " << nodeName << " [label=\"" << opSymbol << "\", shape=circle, width=0.1, height=0.1, margin=0];\n";

                unsigned opIdx = 0;
                for (Use &U : I.operands()) {
                    Value *op = U.get();
                    std::string srcName = nodeNames[op];

                    std::string varName = op->getName().str();
                    varName = (opIdx == 0) ? "op1: " + varName : "op2: " + varName;
                  
                    errs() << "  " << srcName << " -> " << nodeName << " [label=\"" << varName << "\"];\n";
                    opIdx++;
                }
            } else if (isa<StoreInst>(I)) {
                StoreInst *SI = cast<StoreInst>(&I);
                Value *ptr = SI->getPointerOperand();
                Value *val = SI->getValueOperand();

                errs() << "  " << nodeNames[val] << " -> " << outNames[ptr] << " [label=\"" << "op1: " << ptr->getName().str() << "\"];\n";
            } else {
                assert(isa<ReturnInst>(I) && "Unsupported instruction");
            }
        }
    }

    errs() << "}\n";
}

struct DataflowGeneratorPass : public PassInfoMixin<DataflowGeneratorPass> {
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
        visitor(F);
        return PreservedAnalyses::all();
    };
};

}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
    return {
        .APIVersion = LLVM_PLUGIN_API_VERSION,
        .PluginName = "DataflowGenerator",
        .PluginVersion = "v0.1",
        .RegisterPassBuilderCallbacks = [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "dataflow-generator") {
                    FPM.addPass(DataflowGeneratorPass());
                    return true;
                  }
                  return false;
                }
            );
        }
    };
}
