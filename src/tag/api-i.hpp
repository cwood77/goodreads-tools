#ifndef ___tag_api_i___
#define ___tag_api_i___

#include "../cmn/error.hpp"
#include "../db/api.hpp"
#include "api.hpp"
#include <set>
#include <string>

namespace tag {

class iExpert;
class iSet;

namespace impl {

class tagSet : public iSet {
public:
   virtual void foreach(std::function<void(const std::string&)> f)
   { for(auto& e : m_s) f(e); }

   void add(const std::string& s) { m_s.insert(s); }
   size_t size() const { return m_s.size(); }

private:
   std::set<std::string> m_s;
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
   explicit expert(const std::string& tagSyntax) : m_tagSyntax(tagSyntax) {}

   virtual iSet& removeTags(db::listField& f)
   {
      TB tagBuilder(f,m_tagSyntax);
      f.foreach([&](auto& v){ tagBuilder.onValue(v); });
      f.save();
      return tagBuilder.build();
   }

private:
   const std::string m_tagSyntax;
};

class iExpertInfo {
public:
   virtual ~iExpertInfo() {}
   virtual iExpert *tryCreate(const std::string& tagSyntax) const = 0;
};

} // namespace impl
} // namespace tag

#endif // ___tag_api_i___
