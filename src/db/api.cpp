#include "../cmn/error.hpp"
#include "../tcatlib/api.hpp"
#include "api-i.hpp"
#include "api.hpp"
#include <fstream>
#include <memory>

namespace db {
namespace impl {

void item::add(const std::string& k, const std::string& v)
{
   cmn::unimplemented(cdwHere)
      .raise();
}

item& file::add(const std::string& id)
{
   throw cmn::unimplemented(cdwHere)
      .raise();
}

std::vector<std::string> lineParser::split(const std::string& line)
{
   lineParser self(line);
   self.split();
   return self.m_ans;
}

lineParser::lineParser(const std::string& line)
: m_pThumb(line.c_str())
, m_pStart(m_pThumb)
, m_quoted(false)
{
}

void lineParser::split()
{
   for(;*m_pThumb!=0;++m_pThumb)
      if(*m_pThumb == '"')
         m_quoted = !m_quoted;
      else if(*m_pThumb == ',' && !m_quoted)
         addWordIf();

   finish();
}

void lineParser::addWordIf()
{
   if(m_pThumb == m_pStart)
      return;

   std::string word(m_pStart,m_pThumb-m_pStart);
   m_ans.push_back(unquote(word));
   m_pStart = m_pThumb+1;
}

void lineParser::finish()
{
   if(m_quoted)
      cmn::error(cdwHere,"unterminated quote")
         .raise();

   addWordIf();
}

std::string lineParser::unquote(const std::string& q)
{
   if(q.length() < 2)
      return q;
   if(q.c_str()[0] != '"')
      return q;
   if(q.c_str()[q.length()-1] != '"')
      throw cmn::error(cdwHere,"insanity in unquote")
         .with("string",q)
         .raise();
   return std::string(q.c_str()+1,q.length()-2);
}

void fileParser::parse(file& f)
{
   auto columns = lineParser::split(m_lines.getLine());
   auto titleIdx = calculateTitleIndex(columns);

   while(!m_lines.isDone())
   {
      auto values = lineParser::split(m_lines.getLine());
      auto id = values[titleIdx];
      auto& noob = f.add(id);
      for(size_t i=0;i<values.size();i++)
      {
         if(i == titleIdx)
            continue;
         noob.add(columns[i],values[i]);
      }
   }
}

size_t fileParser::calculateTitleIndex(std::vector<std::string>& columns) const
{
   return 0;
}

} // namespace impl
namespace {

class fileManager : public iFileManager {
public:
   virtual iFile& load(const std::string& path) const
   {
      std::ifstream fileStream(path.c_str(),std::ios::binary);
      impl::streamLineProvider lineProv(fileStream);

      impl::fileParser fParser(lineProv);
      std::unique_ptr<impl::file> pFile(new impl::file());
      fParser.parse(*pFile);

      return *pFile.release();
   }

   virtual void mergeInto(iFile& dest, const iFile& source) const
   {
   }

   virtual void saveAs(const iFile& f, const std::string& path) const
   {
   }
};

tcatExposeTypeAs(fileManager,iFileManager);

} // anonymous namespace
} // namespace db
