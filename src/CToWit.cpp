//===- PrintFunctionNames.cpp ---------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Example clang plugin which simply prints the names of all the top-level decls
// in the input file.
//
//===----------------------------------------------------------------------===//

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
  CompilerInstance &Instance;

public:
  PrintFunctionsConsumer(CompilerInstance &Instance) : Instance(Instance) {}

  void HandleTranslationUnit(ASTContext &context) override {
    struct Visitor : public RecursiveASTVisitor<Visitor> {
      Visitor() {}

      bool VisitRecordDecl(RecordDecl *RD) {
        llvm::errs() << RD->getName() << '\n';
        llvm::outs() << "record {" << '\n';
        for (auto i = RD->field_begin(), e = RD->field_end(); i != e; i++) {
          FieldDecl *FD = *i;
          llvm::errs() << *FD << '\n';
          llvm::errs() << FD->getType() << '\n';
        }
        llvm::outs() << "}" << '\n';
        return true;
      }

      bool VisitTypedefDecl(TypedefDecl *TD) { return true; }

      std::set<FunctionDecl *> LateParsedDecls;
    } v;
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
