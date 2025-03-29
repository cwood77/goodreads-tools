#ifndef ___tag_api___
#define ___tag_api___

#include <functional>
#include <string>

namespace db { class listField; }

namespace tag {

class iSet {
public:
   virtual ~iSet() {}
   virtual void foreach(std::function<void(const std::string&)> f) = 0;
};

class iExpert {
public:
   virtual ~iExpert() {}
   virtual iSet& removeTags(db::listField& f) = 0;
};

class iManager {
public:
   virtual ~iManager() {}
   virtual iExpert& createExpert(const std::string& tagSyntax) = 0;
};

} // namespace tag

#endif // ___tag_api___
