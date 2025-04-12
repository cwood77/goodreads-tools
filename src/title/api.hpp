#ifndef ___title_api___
#define ___title_api___

#include <string>

namespace db { class iItem; }

namespace title {

class iExpert {
public:
   virtual ~iExpert() {}
   virtual std::string chooseName(const db::iItem& i, const std::string& tag) const = 0;
};

class iManager {
public:
   virtual ~iManager() {}
   virtual std::string getDefault() const = 0;
   virtual iExpert& createExpert(const std::string& titleSyntax) = 0;
};

} // namespace title

#endif // ___title_api___
