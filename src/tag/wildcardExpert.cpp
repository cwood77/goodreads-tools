#include "../cmn/string.hpp"
#include "../tcatlib/api.hpp"
#include "api-i.hpp"
#include "api.hpp"

namespace tag {
namespace impl {
namespace {

class wildcardTagBuilder : public iTagBuilder {
public:
   virtual void onValue(const std::string& v) = 0;
   virtual iSet& build() = 0;
};

class info : public iExpertInfo {
public:
   virtual iExpert *tryCreate(const std::string& tagSyntax) const
   {
      if(!cmn::endsWith(tagSyntax,"*"))
         return NULL;
      return new expert<wildcardTagBuilder>(tagSyntax);
   }
};

tcatExposeTypeAs(info,iExpertInfo);

} // anonymous namespace
} // namespace impl
} // namespace tag
