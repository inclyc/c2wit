#include "Support.h"
#include <llvm/Support/FormatVariadic.h>

namespace clang::c2wit {

using llvm::formatv;

std::string getWitNameFromBuiltinType(const ASTContext &Cxt,
                                      const BuiltinType *BT) {
  if (BT->isInteger())
    return formatv("{0}{1}", BT->isSignedInteger() ? "i" : "u",
                   Cxt.getTypeSize(BT));
  if (BT->isFloatingType())
    return formatv("f{0}", Cxt.getTypeSize(BT));
  return "????";
}

} // namespace clang::c2wit
