#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Sema/Sema.h"
#include "llvm/Support/raw_ostream.h"
using namespace clang;

namespace {

class PrintFunctionsConsumer : public ASTConsumer {
  CompilerInstance &CI;

public:
  PrintFunctionsConsumer(CompilerInstance &CI) : CI(CI) {}

  void HandleTranslationUnit(ASTContext &context) override {
    struct Visitor : public RecursiveASTVisitor<Visitor> {
      CompilerInstance &CI;
      Visitor(CompilerInstance &CI) : CI(CI) {}

      bool VisitRecordDecl(RecordDecl *RD) {
        llvm::errs() << RD->getName() << '\n';
        llvm::outs() << "record {" << '\n';
        for (auto i = RD->field_begin(), e = RD->field_end(); i != e; i++) {
          FieldDecl *FD = *i;
          llvm::outs() << FD->getName() << ": ";
          if (const auto *BT = FD->getType()->getAs<BuiltinType>()) {
            if (BT->isInteger()) {
              llvm::outs() << (BT->isSignedInteger() ? "i" : "u");
              llvm::outs() << CI.getASTContext().getTypeSize(BT) << "\n";
            } else if (BT->isFloatingType()) {
              llvm::outs() << "f" << CI.getASTContext().getTypeSize(BT) << "\n";
            }
          } else {
            llvm::outs() << "????" << '\n';
          }
        }
        llvm::outs() << "}" << '\n';
        return true;
      }

      std::set<FunctionDecl *> LateParsedDecls;
    } v(CI);
    v.TraverseDecl(context.getTranslationUnitDecl());
  }
};

class PrintFunctionNamesAction : public PluginASTAction {

protected:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 llvm::StringRef) override {
    return std::make_unique<PrintFunctionsConsumer>(CI);
  }

  bool ParseArgs(const CompilerInstance &,
                 const std::vector<std::string> &) override {
    return true;
  }
};

} // namespace

static FrontendPluginRegistry::Add<PrintFunctionNamesAction>
    X("ctowit", "Convert C structs to wit");
