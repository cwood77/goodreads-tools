#ifndef ___expert_api___
#define ___expert_api___

#include "../cmn/error.hpp"
#include "../tcatlib/api.hpp"
#include <memory>
#include <string>

namespace expert {

template<class E>
class iExpertInfo {
public:
   typedef E expertType;

   virtual ~iExpertInfo() {}
   virtual E *tryCreate(const std::string& syntax) const = 0;
};

template<class EI>
class managerHelper {
public:
   virtual typename EI::expertType& createExpert(const std::string& syntax)
   {
      std::unique_ptr<typename EI::expertType> pEx;
      for(size_t i=0;i<m_infos.size();i++)
      {
         auto *pNewEx = m_infos[i]->tryCreate(syntax);
         if(!pNewEx)
            continue;

         if(pEx.get())
         {
            delete pNewEx;
            throw cmn::error(cdwHere,"multiple experts claim support for syntax")
               .with("syntax",syntax)
               .raise();
         }
         else
            pEx.reset(pNewEx);
      }

      if(!pEx.get())
         throw cmn::error(cdwHere,"no expert claim support for syntax")
            .with("syntax",syntax)
            .raise();
      return *pEx.release();
   }

private:
   tcat::typeSet<EI> m_infos;
};

} // namespace expert

#endif // ___expert_api___
