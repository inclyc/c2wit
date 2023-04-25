#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Sema/Sema.h"
#include "llvm/Support/raw_ostream.h"
using namespace clang;

namespace {

class C2WitConsumer : public ASTConsumer {
  CompilerInstance &CI;

public:
  C2WitConsumer(CompilerInstance &CI) : CI(CI) {}

  void HandleTranslationUnit(ASTContext &ACxt) override {
    struct Visitor : public RecursiveASTVisitor<Visitor> {
      CompilerInstance &CI;
      Visitor(CompilerInstance &CI) : CI(CI) {}

      bool VisitRecordDecl(RecordDecl *RD) {
        llvm::outs() << "record " << RD->getName() << " {\n";
        for (auto I = RD->field_begin(), E = RD->field_end(); I != E; I++) {
          FieldDecl *FD = *I;
          llvm::outs() << FD->getName() << ": ";
          if (const auto *BT = FD->getType()->getAs<BuiltinType>()) {
            if (BT->isInteger())
              llvm::outs() << (BT->isSignedInteger() ? "i" : "u")
                           << CI.getASTContext().getTypeSize(BT);
            else if (BT->isFloatingType())
              llvm::outs() << "f" << CI.getASTContext().getTypeSize(BT);
          } else {
            llvm::outs() << "????";
          }
          llvm::outs() << ",\n";
        }
        llvm::outs() << "}" << '\n';
        return true;
      }

      std::set<FunctionDecl *> LateParsedDecls;
    } V(CI);
    V.TraverseDecl(ACxt.getTranslationUnitDecl());
  }
};

class C2WitASTAction : public PluginASTAction {

protected:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 llvm::StringRef) override {
    return std::make_unique<C2WitConsumer>(CI);
  }

  bool ParseArgs(const CompilerInstance &,
                 const std::vector<std::string> &) override {
    return true;
  }
};

} // namespace

static FrontendPluginRegistry::Add<C2WitASTAction>
    X("c2wit", "Convert C structs to wit");
