#include "../cmn/string.hpp"
#include "../db/api.hpp"
#include "../tcatlib/api.hpp"
#include "api-i.hpp"
#include "api.hpp"
#include <memory>

namespace tag {
namespace impl {
namespace {

class stdTagBuilder : public iTagBuilder {
public:
   stdTagBuilder(db::listField& f, const std::string&)
   : m_f(f), m_pSet(new tagSet())
   {
      m_legalValues.insert("to-read");
      m_legalValues.insert("currently-reading");
      m_legalValues.insert("read");
   }

   virtual void onValue(const std::string& v)
   {
      if(m_legalValues.find(v) == m_legalValues.end())
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
         m_pSet->add("HOW");
   }

   db::listField& m_f;
   std::set<std::string> m_legalValues;
   std::unique_ptr<tagSet> m_pSet;
};

class info : public iExpertInfo {
public:
   virtual iExpert *tryCreate(const std::string& syntax) const
   {
      if(syntax != "std")
         return NULL;
      return new myExpert<stdTagBuilder>(syntax);
   }
};

tcatExposeTypeAs(info,iExpertInfo);

} // anonymous namespace
} // namespace impl
} // namespace tag
