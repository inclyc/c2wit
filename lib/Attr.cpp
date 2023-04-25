#include "Attr.h"

namespace clang::c2wit {
static ParsedAttrInfoRegistry::Add<WitAttrInfo>
    Z("example_attr", "example attribute description");

WitAttrInfo::AttrHandling
WitAttrInfo::handleDeclAttribute(Sema &S, Decl *D,
                                 const ParsedAttr &Attr) const {
  return AttributeApplied;
}
} // namespace clang::c2wit
