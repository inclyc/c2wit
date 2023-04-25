#include <clang/AST/ASTContext.h>
#include <clang/AST/Type.h>
#include <string>

namespace clang::c2wit {

std::string getWitNameFromBuiltinType(const ASTContext &Cxt,
                                      const BuiltinType *BT);

} // namespace clang::c2wit
