#include "Consumer.h"
#include "Support.h"
namespace clang::c2wit {
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
} // namespace clang::c2wit
