#ifndef ___survey_api___
#define ___survey_api___

#include <functional>
#include <string>

namespace survey {

class iCatalog {
public:
   virtual ~iCatalog() {}
   virtual void foreach(std::function<void(const std::string&,const std::string&)> f) = 0;
};

class iScanner {
public:
   virtual ~iScanner() {}
   virtual iCatalog& scan(const std::string& rootPath) = 0;
};

} // namespace survey

#endif // ___survey_api___
