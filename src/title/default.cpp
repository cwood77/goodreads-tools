#include "../db/api.hpp"
#include "../tcatlib/api.hpp"
#include "api-i.hpp"
#include "api.hpp"
#include "sanitizer.hpp"

namespace title {
namespace impl {
namespace {

class myExpert : public iExpert {
public:
   explicit myExpert(const std::string& syntax) : m_syntax(syntax) {}

   virtual std::string chooseName(const db::iItem& i, const std::string& tag) const
   {
      std::string fileName
         = i.demand("Title")
         + " ("
         + i.demand("Author")
         + ") #"
         + i.demand("Book Id")
      ;
      std::string folder = tag + "\\" + sanitize_file_word(fileName) + ".csv";
      return folder;
   }

private:
   const std::string m_syntax;
};

class info : public iExpertInfo {
public:
   virtual iExpert *tryCreate(const std::string& syntax) const
   {
      if(!syntax.empty())
         return NULL;
      return new myExpert(syntax);
   }
};

tcatExposeTypeAs(info,iExpertInfo);

} // anonymous namespace
} // namespace impl
} // namespace title
