#pragma once

#include <clang/AST/AST.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Sema/Sema.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/raw_ostream.h>
namespace clang::c2wit {

class C2WitConsumer : public ASTConsumer,
                      public RecursiveASTVisitor<C2WitConsumer> {
  CompilerInstance &CI;

public:
  C2WitConsumer(CompilerInstance &CI, std::unique_ptr<raw_ostream> OS)
      : CI(CI), OS(OS ? *OS : llvm::outs()), OwnedOS(std::move(OS)) {}

  void HandleTranslationUnit(ASTContext &ACxt) override;
  bool VisitRecordDecl(RecordDecl *RD);
  bool VisitFunctionDecl(FunctionDecl *D);

  struct WitAnnotation {
    static constexpr llvm::StringRef Export = "wit-export";
    static constexpr llvm::StringRef Name = "wit-name";
  };

private:
  llvm::raw_ostream &OS;
  std::unique_ptr<llvm::raw_ostream> OwnedOS;
};

} // namespace clang::c2wit
