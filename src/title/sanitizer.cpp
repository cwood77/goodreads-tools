#include "sanitizer.hpp"
#include <cctype>
#include <map>

namespace title {
namespace impl {
namespace {

class sanitizer {
public:
   explicit sanitizer(char replacement = '_') : m_replacement(replacement)
   {
      prohibitChar(':',';');
      prohibitChar('\\',',');
      prohibitChar('/',',');

      prohibitChar('<');
      prohibitChar('>');
      prohibitChar('|');

      prohibitChar('"','\'');
      prohibitChar('?');
      prohibitChar('*','x');
   }

   std::string replaceIf(const std::string& in) const
   {
      std::string copy = in.c_str();

      for(size_t i=0;i<copy.length();i++)
         replaceIf(const_cast<char&>(copy.c_str()[i]));

      return copy;
   }

private:
   void prohibitChar(char c, char r) { m_prohibited[c] = r; }
   void prohibitChar(char c) { prohibitChar(c,m_replacement); }

   void replaceIf(char& c) const
   {
      if(!std::isprint(c))
      {
         c = m_replacement;
         return;
      }

      auto it = m_prohibited.find(c);
      if(it != m_prohibited.end())
         c = it->second;
   }

   std::map<char,char> m_prohibited;
   const char m_replacement;
};

} // anonymous namespace

std::string sanitize_file_word(const std::string& in)
{
   static sanitizer _san;
   return _san.replaceIf(in);
}

} // namespace impl
} // namespace title

