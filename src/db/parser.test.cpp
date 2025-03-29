#ifdef cdwTest
#include "../tcatlib/api.hpp"
#include "../test/api.hpp"
#include "api-i.hpp"

namespace db {
namespace impl {
namespace {

testDefineTest(fileParser_splitLine_lastWord)
{
   std::vector<std::string> expected;
   expected.push_back("a");

   std::vector<std::string> actual = lineParser::split("a");
   a.assertTrue(expected == actual);
}

testDefineTest(fileParser_splitLine_multiWord)
{
   std::vector<std::string> expected;
   expected.push_back("a");
   expected.push_back("b c");

   std::vector<std::string> actual = lineParser::split("a,b c");
   a.assertTrue(expected == actual);
}

testDefineTest(fileParser_splitLine_multiWord_escaped)
{
   std::vector<std::string> expected;
   expected.push_back("a");
   expected.push_back("b,c");
   expected.push_back("d");

   std::vector<std::string> actual = lineParser::split("a,\"b,c\",d");
   a.assertTrue(expected == actual);
}

testDefineTest(fileParser_splitLine_multiWord_empty)
{
   std::vector<std::string> expected;
   expected.push_back("");
   expected.push_back("");
   expected.push_back("");

   std::vector<std::string> actual = lineParser::split(",,");
   a.assertTrue(expected == actual);
}

} // anonymous namespace
} // namespace impl
} // namespace db

#endif // cdwTest
