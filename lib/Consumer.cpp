#include "Consumer.h"
#include "Support.h"
#include <clang/AST/ASTContext.h>
#include <clang/AST/Attrs.inc>
#include <clang/AST/Expr.h>
#include <clang/AST/Type.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringExtras.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/Casting.h>
#include <llvm/Support/Errc.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/FormatVariadic.h>
#include <optional>
namespace clang::c2wit {

using llvm::StringRef;

void C2WitConsumer::HandleTranslationUnit(clang::ASTContext &ACxt) {
  TraverseDecl(ACxt.getTranslationUnitDecl());
}

bool C2WitConsumer::VisitRecordDecl(RecordDecl *RD) {
  OS << "record " << RD->getName() << " {\n";
  for (auto *FD : RD->fields()) {
    OS << FD->getName() << ": ";
    if (const auto *BT = FD->getType()->getAs<BuiltinType>())
      OS << getWitNameFromBuiltinType(CI.getASTContext(), BT);
    else
      OS << "????";
    OS << ",\n";
  }
  OS << "}" << '\n';
  return true;
}

static const StringLiteral *maybeConstEvalStringLiteral(ASTContext &Context,
                                                        const Expr *E) {
  Expr::EvalResult Result;
  if (E->EvaluateAsRValue(Result, Context) && Result.Val.isLValue()) {
    const auto *LVE = Result.Val.getLValueBase().dyn_cast<const Expr *>();
    if (const StringLiteral *SLE = llvm::dyn_cast<StringLiteral>(LVE))
      return SLE;
  }
  return nullptr;
}

static std::optional<StringRef>
extractNameAnnotation(const AnnotateAttr *AA, ASTContext &Context, Sema &S) {
  if (AA->args_size() != 1) {
    unsigned ID = S.getDiagnostics().getCustomDiagID(
        DiagnosticsEngine::Error,
        "naming annotate attribute only accept 1 argument exactly");
    S.Diag(AA->getLoc(), ID);
    return std::nullopt;
  }
  Expr *AName = *AA->args_begin();
  if (auto *AStringLiteral = maybeConstEvalStringLiteral(Context, AName))
    return AStringLiteral->getString();

  unsigned ID = S.getDiagnostics().getCustomDiagID(
      DiagnosticsEngine::Error, "Cannot evaluate naming argument to string");
  S.Diag(AA->getLoc(), ID);
  return std::nullopt;
}

bool C2WitConsumer::VisitFunctionDecl(FunctionDecl *D) {
  // https://github.com/WebAssembly/component-model/blob/79c0614c55b06896af3349f5dadf644fb077922b/design/mvp/WIT.md?plain=1#L703
  // func-item ::= id ':' func-type
  // func-type ::= 'func' param-list result-list
  // param-list ::= '(' named-type-list ')'
  // result-list ::= ϵ
  //               | '->' ty
  //               | '->' '(' named-type-list ')'
  // named-type-list ::= ϵ
  //                   | named-type ( ',' named-type )*
  // named-type ::= id ':' ty
  AttrVec Attrs = D->getAttrs();
  bool ShouldExport = false;
  StringRef Name;
  for (const Attr *Attr : Attrs) {
    if (const AnnotateAttr *AA = llvm::dyn_cast<AnnotateAttr>(Attr)) {
      StringRef Annotation = AA->getAnnotation();
      if (Annotation == /* "wit-export" */ WitAnnotation::Export)
        ShouldExport = true;
      else if (Annotation == WitAnnotation::Name) {
        if (std::optional<llvm::StringRef> OptName =
                extractNameAnnotation(AA, CI.getASTContext(), CI.getSema()))
          Name = *OptName;
        else
          return true;
      }
    }
  }
  if (!ShouldExport)
    return true;

  // Export this function
  if (Name.empty())
    Name = D->getName();

  // func-item ::= id ':' func-type
  OS << /* id */ Name << ": "
     << "func";

  // func-type ::= 'func' param-list result-list
  {

    // param-list ::= '(' named-type-list ')'
    {
      OS << "(";
      llvm::SmallVector<std::string, 0> NamedTypeList;
      for (const ParmVarDecl *PD : D->parameters()) {
        // TOOD: support atrribute rename

        // identifier
        const StringRef ParamName = PD->getName();

        // typename
        const auto Ty = PD->getOriginalType();
        std::string ParamTyName;
        if (const auto *const BT = Ty->getAs<BuiltinType>()) {
          ParamTyName = getWitNameFromBuiltinType(CI.getASTContext(), BT);
        } else {
          // TODO: random name?
          assert(0 && "NYI");
        }
        assert(ParamTyName.length() != 0 && "Must give a non-empty typename!");

        // named-type ::= id ':' ty
        NamedTypeList.push_back(
            llvm::formatv("{0}: {1}", ParamName, ParamTyName));
      }
      // named-type ( ',' named-type )*
      OS << llvm::join(NamedTypeList, ", ");
      OS << ")";
    }

    // result-list ::= ϵ
    //               | '->' ty
    //               | '->' '(' named-type-list ')'
    {
      QualType ReturnTy = D->getReturnType();
      if (!ReturnTy->isVoidType()) {
        OS << " -> ";
        // TODO: tuple -> named type list?
        OS << getWitNameFromBuiltinType(CI.getASTContext(),
                                        ReturnTy->getAs<BuiltinType>());
      }
    }
  }
  OS << "\n";
  return true;
}

} // namespace clang::c2wit
