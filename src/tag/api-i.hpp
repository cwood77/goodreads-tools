#ifndef ___tag_api_i___
#define ___tag_api_i___

#include "../cmn/error.hpp"
#include "api.hpp"
#include <string>

namespace tag {

class iExpert;
class iSet;

namespace impl {

class tagSet : public iSet {
public:
   void add(const std::string& s);
   size_t size() const;
};

class iTagBuilder {
public:
   virtual ~iTagBuilder() {}
   virtual void onValue(const std::string& v) = 0;
   virtual iSet& build() = 0;
};

template<class TB>
class expert : public iExpert {
public:
   explicit expert(const std::string& tagSyntax) {}

   virtual iSet& removeTags(db::listField& f)
   {
      throw cmn::unimplemented(cdwHere).raise();
   }
};

class iExpertInfo {
public:
   virtual ~iExpertInfo() {}
   virtual iExpert *tryCreate(const std::string& tagSyntax) const = 0;
};

} // namespace impl
} // namespace tag

#endif // ___tag_api_i___
