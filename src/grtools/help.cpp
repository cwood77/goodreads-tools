#include "../console/arg.hpp"
#include "../console/log.hpp"
#include "../tcatlib/api.hpp"

namespace {

console::helpVerbs gHelp([](console::iLog& l)
{
   l.writeLnInfo("");
   l.writeLnInfo("grtools - tools for working with goodreads databases");
   l.writeLnInfo("");
   l.writeLnInfo("verbs and switches");
});

} // anonymous namespace
