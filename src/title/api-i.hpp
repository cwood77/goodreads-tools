#ifndef ___title_api_i___
#define ___title_api_i___

#include "../db/api.hpp"
#include "../expert/api.hpp"
#include <string>

namespace title {

class iExpert;

namespace impl {

class iExpertInfo : public expert::iExpertInfo<iExpert> {};
typedef expert::managerHelper<iExpertInfo> managerHelper;

} // namespace impl
} // namespace title

#endif // ___title_api_i___
