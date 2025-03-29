#include "../cmn/error.hpp"
#include "../tcatlib/api.hpp"
#include "api-i.hpp"
#include "api.hpp"
#include <cstring>
#include <fstream>
#include <memory>

namespace db {
namespace impl {

void item::addNew(const std::string& k, const std::string& v)
{
   auto it = m_values.find(k);
   if(it != m_values.end())
      cmn::error(cdwHere,"field set twice")
         .with("key",k)
         .raise();
   m_values[k] = v;
}

std::string& item::demand(const std::string& k)
{
   auto it = m_values.find(k);
   if(it == m_values.end())
      cmn::error(cdwHere,"required field absent")
         .with("key",k)
         .raise();
   return it->second;
}

const std::string& item::demand(const std::string& k) const
{
   return const_cast<item&>(*this).demand(k);
}

iFile& item::cloneNewFile() const
{
   std::unique_ptr<file> pClone(new file());
   pClone->columns() = m_file.columns();
   auto& i = pClone->addNew(demand("Book Id"));
   i.m_values = m_values;
   return *pClone.release();
}

void item::removeColumn(const std::string& key)
{
   m_values.erase(key);
}

file::~file()
{
   for(auto it=m_items.begin();it!=m_items.end();++it)
      delete it->second;
}

void file::removeColumn(const std::string& key)
{
   auto idx = findIndex(m_columns,key);
   m_columns.erase(m_columns.begin()+idx);
   for(auto it=m_items.begin();it!=m_items.end();++it)
      it->second->removeColumn(key);
}

void file::foreach(std::function<void(iItem&)> f)
{
   for(auto it=m_items.begin();it!=m_items.end();++it)
      f(*it->second);
}

item& file::addNew(const std::string& id)
{
   auto*& pItem = m_items[id];
   if(pItem)
      throw cmn::error(cdwHere,"item added twice")
         .with("id",id)
         .raise();
   pItem = new item(*this);
   m_order.push_back(id);
   return *pItem;
}

void file::ordered(std::function<void(const item&)> f) const
{
   if(m_order.size() != m_items.size())
      cmn::error(cdwHere,"insanity")
         .raise();

   for(auto id : m_order)
      f(*m_items[id]);
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
, m_macroQuote(false)
{
}

void lineParser::split()
{
   checkWordStart();

   for(;*m_pThumb!=0;++m_pThumb)
   {
      if(*m_pThumb == '"' && !m_macroQuote)
         m_quoted = !m_quoted;
      else if(*m_pThumb == ',' && !m_quoted)
         addWordIf();
   }

   finish();
}

void lineParser::checkWordStart()
{
   if(!m_macroQuote && ::strncmp(m_pStart,"\"=\"\"",4)==0)
      m_macroQuote = true;
}

void lineParser::checkWordEnd()
{
   if(m_macroQuote && ::strncmp(m_pThumb-3,"\"\"\"",3)==0)
      m_macroQuote = false;
}

void lineParser::addWordIf()
{
   std::string word(m_pStart,m_pThumb-m_pStart);
   m_ans.push_back(unquote(word));
   checkWordEnd();
   m_pStart = m_pThumb+1;
   if(*m_pThumb)
      checkWordStart();
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
   if(m_macroQuote)
      return q;

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
   auto titleIdx = findIndex(columns,std::string("Book Id"));

   while(!m_lines.isDone())
   {
      auto line = m_lines.getLine();
      if(line.empty())
         continue;
      auto values = lineParser::split(line);
      if(values.size() != columns.size())
         cmn::error(cdwHere,"line has wrong element size")
            .with("expected size",columns.size())
            .with("actual size",values.size())
            .raise();
      auto id = values[titleIdx];
      auto& noob = f.addNew(id);
      for(size_t i=0;i<values.size();i++)
         noob.addNew(columns[i],values[i]);
   }

   f.columns() = columns;
}

lineFormatter& lineFormatter::append(const std::string& s)
{
   if(m_first)
      m_first = !m_first;
   else
      m_stream << ",";
   m_stream << quoteIf(s);
   return *this;
}

lineFormatter& lineFormatter::append(const std::vector<std::string>& v)
{
   for(auto& e : v)
      append(e);
   return *this;
}

std::string lineFormatter::quoteIf(const std::string& s)
{
   if(s.find(',') == s.npos)
      return s;
   return std::string("\"") + s + std::string("\"");
}

void fileFormatter::format(const file& f)
{
   lineFormatter(m_stream).append(f.columns());
   m_stream << std::endl;

   f.ordered([&](auto& i)
   {
      format(f.columns(),i);
   });
}

void fileFormatter::format(const std::vector<std::string>& cols, const item& i)
{
   lineFormatter lf(m_stream);
   for(auto c : cols)
      lf.append(i.demand(c));
   m_stream << std::endl;
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
      cmn::unimplemented(cdwHere).raise();
   }

   virtual void saveAs(const iFile& f, const std::string& path) const
   {
      std::ofstream fileStream(path.c_str(),std::ios::out|std::ios::binary);
      impl::fileFormatter(fileStream).format(dynamic_cast<const impl::file&>(f));
   }
};

tcatExposeTypeAs(fileManager,iFileManager);

} // anonymous namespace
} // namespace db
