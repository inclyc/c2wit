#include "Attr.h"
#include "Consumer.h"
#include <clang/Frontend/FrontendPluginRegistry.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/raw_ostream.h>
#include <memory>

namespace clang::c2wit {

using namespace llvm;

static cl::OptionCategory C2WitCategory("c2wit options");

cl::opt<std::string> OutputFilename("o", cl::desc("Specify output filename"),
                                    cl::value_desc("filename"), cl::init("-"),
                                    cl::cat(C2WitCategory));

class C2WitASTAction : public PluginASTAction {
protected:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 llvm::StringRef) override {
    std::unique_ptr<raw_ostream> OS =
        OutputFilename == "-"
            ? nullptr
            : CI.createOutputFile(OutputFilename, false, true, false);
    return std::make_unique<C2WitConsumer>(CI, nullptr);
  }

  bool ParseArgs(const CompilerInstance &,
                 const std::vector<std::string> &PluginArgs) override {
    auto CLIArgs = PluginArgs;
    // Insert fake "c2wit" as $0
    CLIArgs.insert(CLIArgs.begin(), "c2wit");
    std::vector<const char *> Argv(CLIArgs.size());
    std::transform(CLIArgs.begin(), CLIArgs.end(), Argv.begin(),
                   [](const std::string &S) { return S.c_str(); });
    return cl::ParseCommandLineOptions(CLIArgs.size(), Argv.data());
  }
};

static FrontendPluginRegistry::Add<C2WitASTAction>
    X("c2wit", "Convert C structs to wit");
} // namespace clang::c2wit
