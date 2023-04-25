#include "Support.h"
#include <fmt/core.h>

namespace clang::c2wit {

std::string getWitNameFromBuiltinType(const ASTContext &Cxt,
                                      const BuiltinType *BT) {
  if (BT->isInteger())
    return fmt::format("{}{}", BT->isSignedInteger() ? "i" : "u",
                       Cxt.getTypeSize(BT));
  if (BT->isFloatingType())
    return fmt::format("f{}", Cxt.getTypeSize(BT));
  return "????";
}

} // namespace clang::c2wit
