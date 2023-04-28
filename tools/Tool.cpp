#include "Consumer.h"
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/CommandLine.h>

using namespace clang::tooling;
using namespace clang;
using namespace llvm;

static llvm::cl::OptionCategory C2WitCategory("c2wit options");
cl::opt<std::string> OutputFilename("o", cl::desc("Specify output filename"),
                                    cl::value_desc("filename"), cl::init("-"),
                                    cl::cat(C2WitCategory));

namespace clang::c2wit {

class C2WitASTAction : public ASTFrontendAction {
protected:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 llvm::StringRef) override {
    std::unique_ptr<raw_ostream> OS =
        OutputFilename == "-"
            ? nullptr
            : CI.createOutputFile(OutputFilename, false, true, false);
    return std::make_unique<C2WitConsumer>(CI, std::move(OS));
  }
};

} // namespace clang::c2wit

int main(int argc, const char **argv) {
  auto ExpectedParser = CommonOptionsParser::create(argc, argv, C2WitCategory);
  if (!ExpectedParser) {
    llvm::errs() << ExpectedParser.takeError();
    return 1;
  }

  CommonOptionsParser &OptionsParser = ExpectedParser.get();
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  Tool.run(newFrontendActionFactory<clang::c2wit::C2WitASTAction>().get());
}
