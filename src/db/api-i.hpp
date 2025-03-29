#ifndef ___db_api_i___
#define ___db_api_i___

#include "api.hpp"
#include <map>
#include <string>
#include <vector>

namespace db {
namespace impl {

class item {
public:
   void add(const std::string& k, const std::string& v);

private:
   std::map<std::string,std::string> values;
};

class file : public iFile {
public:
   item& add(const std::string& id);

private:
   const std::string idField;
   std::map<std::string,item*> items;
   std::list<std::string> order;
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
   void addWordIf();
   void finish();

   std::string unquote(const std::string& q);

   const char *m_pThumb;
   const char *m_pStart;
   bool m_quoted;
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
   size_t calculateTitleIndex(std::vector<std::string>& columns) const;
   //void parseLine(const std::string& line);

   iLineProvider& m_lines;
};

} // namespace impl
} // namespace db

#endif // ___db_api_i___
