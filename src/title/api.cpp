#include "../tcatlib/api.hpp"
#include "api-i.hpp"
#include "api.hpp"

namespace title {
namespace {

class manager : public iManager {
public:
   virtual std::string getDefault() const { return ""; }

   virtual iExpert& createExpert(const std::string& syntax)
   { return m_manHelp.createExpert(syntax); }

private:
   impl::managerHelper m_manHelp;
};

tcatExposeTypeAs(manager,iManager);

} // anonymous namespace
} // namespace title
