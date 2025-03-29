#ifndef ___db_api_i___
#define ___db_api_i___

#include "../cmn/error.hpp"
#include "api.hpp"
#include <functional>
#include <map>
#include <string>
#include <vector>

namespace db {
namespace impl {

class file;

template<class T>
inline size_t findIndex(std::vector<T>& v, const T& e)
{
   for(size_t i=0;i<v.size();i++)
      if(v[i] == e)
         return i;

   throw cmn::error(cdwHere,"findIndex failed")
      .with("elt",e)
      .raise();
}

class item : public iItem {
public:
   explicit item(file& f) : m_file(f) {}

   void addNew(const std::string& k, const std::string& v);

   virtual std::string& demand(const std::string& k);
   virtual const std::string& demand(const std::string& k) const;
   virtual iFile& cloneNewFile() const;

   void removeColumn(const std::string& key);

private:
   file& m_file;
   std::map<std::string,std::string> m_values;
};

class file : public iFile {
public:
   virtual ~file();

   virtual void removeColumn(const std::string& key);
   virtual void foreach(std::function<void(iItem&)> f);

   std::vector<std::string>& columns()             { return m_columns; }
   const std::vector<std::string>& columns() const { return m_columns; }
   item& addNew(const std::string& id);
   //void reorder(const file& proto);

   void ordered(std::function<void(const item&)> f) const;

private:
   std::vector<std::string> m_columns;
   mutable std::map<std::string,item*> m_items;
   std::list<std::string> m_order;
};

class iLineProvider {
public:
   virtual std::string getLine() = 0;
   virtual bool isDone() const = 0;
};

class lineParser {
public:
   static std::vector<std::string> split(const std::string& line);

private:
   explicit lineParser(const std::string& line);

   void split();
   void checkWordStart();
   void checkWordEnd();
   void addWordIf();
   void finish();

   std::string unquote(const std::string& q);

   const char *m_pThumb;
   const char *m_pStart;
   bool m_quoted;
   bool m_macroQuote;
   std::vector<std::string> m_ans;
};

class streamLineProvider : public iLineProvider {
public:
   explicit streamLineProvider(std::istream& s) : m_s(s) {}

   virtual std::string getLine()
   {
      std::string str;
      std::getline(m_s,str);
      return str;
   }

   virtual bool isDone() const
   {
      return !m_s.good();
   }

private:
   std::istream& m_s;
};

class fileParser {
public:
   explicit fileParser(iLineProvider& lines) : m_lines(lines) {}

   void parse(file& f);

private:
   //void parseLine(const std::string& line);

   iLineProvider& m_lines;
};

class lineFormatter {
public:
   explicit lineFormatter(std::ostream& s) : m_stream(s), m_first(true) {}

   lineFormatter& append(const std::string& s);
   lineFormatter& append(const std::vector<std::string>& v);

private:
   static std::string quoteIf(const std::string& s);

   std::ostream& m_stream;
   bool m_first;
};

class fileFormatter {
public:
   explicit fileFormatter(std::ostream& s) : m_stream(s) {}

   void format(const file& f);

private:
   void format(const std::vector<std::string>& cols, const item& i);

   std::ostream& m_stream;
};

} // namespace impl
} // namespace db

#endif // ___db_api_i___
