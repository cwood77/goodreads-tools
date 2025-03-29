#include "../cmn/error.hpp"
#include "../tcatlib/api.hpp"
#include "api-i.hpp"
#include "api.hpp"
#include <memory>

namespace tag {
namespace {

class manager : public iManager {
public:
   virtual iExpert& createExpert(const std::string& tagSyntax)
   {
      std::unique_ptr<iExpert> pEx;
      for(size_t i=0;i<m_infos.size();i++)
      {
         auto *pNewEx = m_infos[i]->tryCreate(tagSyntax);
         if(!pNewEx)
            continue;

         if(pEx.get())
         {
            delete pNewEx;
            throw cmn::error(cdwHere,"multiple experts claim support for syntax")
               .with("syntax",tagSyntax)
               .raise();
         }
         else
            pEx.reset(pNewEx);
      }

      if(!pEx.get())
         throw cmn::error(cdwHere,"no expert claim support for syntax")
            .with("syntax",tagSyntax)
            .raise();
      return *pEx.release();
   }

private:
   tcat::typeSet<impl::iExpertInfo> m_infos;
};

tcatExposeTypeAs(manager,iManager);

} // anonymous namespace
} // namespace tag
