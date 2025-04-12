#include "../cmn/string.hpp"
#include "../db/api.hpp"
#include "../tcatlib/api.hpp"
#include "api-i.hpp"
#include "api.hpp"
#include <memory>

namespace tag {
namespace impl {
namespace {

class prefixTagBuilder : public iTagBuilder {
public:
   prefixTagBuilder(db::listField& f, const std::string& prefix)
   : m_f(f), m_prefix(prefix), m_pSet(new tagSet()) {}

   virtual void onValue(const std::string& v)
   {
      if(!cmn::startsWith(v,m_prefix))
         return;
      m_pSet->add(v);
      m_f.erase(v);
   }

   virtual iSet& build()
   {
      addDefaultIf();
      return *m_pSet.release();
   }

private:
   void addDefaultIf()
   {
      if(m_pSet->size() == 0)
         m_pSet->add(m_prefix + "unset");
   }

   db::listField& m_f;
   std::string m_prefix;
   std::unique_ptr<tagSet> m_pSet;
};

class info : public iExpertInfo {
public:
   virtual iExpert *tryCreate(const std::string& tagSyntax) const
   {
      if(!cmn::endsWith(tagSyntax,"*"))
         return NULL;
      return new myExpert<prefixTagBuilder>(
         std::string(tagSyntax.c_str(),tagSyntax.length()-1));
   }
};

tcatExposeTypeAs(info,iExpertInfo);

} // anonymous namespace
} // namespace impl
} // namespace tag
