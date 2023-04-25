#pragma once
#include <clang/Sema/ParsedAttr.h>
namespace clang::c2wit {

struct WitAttrInfo : ParsedAttrInfo {
  WitAttrInfo() {
    static constexpr Spelling S[] = {{ParsedAttr::AS_GNU, "wit"},
                                     {ParsedAttr::AS_C2x, "wit"},
                                     {ParsedAttr::AS_CXX11, "wit"},
                                     {ParsedAttr::AS_CXX11, "c2wit::wit"}};
    Spellings = S;
  }
  AttrHandling handleDeclAttribute(Sema &S, Decl *D,
                                   const ParsedAttr &Attr) const override;
};

std::string getWitNameFromBuiltinType(BuiltinType *BT);

} // namespace clang::c2wit
