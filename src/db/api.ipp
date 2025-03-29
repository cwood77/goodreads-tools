namespace impl {

class listParser {
public:
   static void split(const std::string& line, std::set<std::string>& values);

private:
   listParser(const std::string& line, std::set<std::string>& values);

   void split();
   void addWord();
   void eatWhitespace();

   const char *m_pThumb;
   const char *m_pStart;
   std::set<std::string>& m_ans;
};

inline void listParser::split(const std::string& line, std::set<std::string>& values )
{
   listParser(line,values).split();
}

inline listParser::listParser(const std::string& line, std::set<std::string>& values)
: m_pThumb(line.c_str())
, m_pStart(m_pThumb)
, m_ans(values)
{
}

inline void listParser::split()
{
   eatWhitespace();

   while(*m_pThumb!=0)
      if(*m_pThumb == ',')
         addWord();
      else
         ++m_pThumb;

   addWord();
}

inline void listParser::addWord()
{
   std::string word(m_pStart,m_pThumb-m_pStart);
   m_ans.insert(word);

   m_pThumb++;
   m_pStart = m_pThumb;

   eatWhitespace();
}

inline void listParser::eatWhitespace()
{
   for(;*m_pThumb==' ';++m_pThumb);
   m_pStart = m_pThumb;
}

} // namespace impl

inline listField::listField(std::string& s)
: m_backingStore(s)
{
   impl::listParser::split(m_backingStore,m_values);
}

inline listField& listField::foreach(std::function<void(const std::string&)> f)
{
   for(auto& e : m_values)
      f(e);
   return *this;
}

inline listField& listField::erase(const std::string& value)
{
   m_values.erase(value);
   return *this;
}

inline listField& listField::sort()
{
   // this does nothing, because things are already sorted by-design
   // the API is exposed just to make the callers intent explicit
   return *this;
}

inline listField& listField::save()
{
   std::stringstream stream;

   bool first = true;
   for(auto& v : m_values)
   {
      if(first)
         first = false;
      else
         stream << ", ";
      stream << v;
   }

   m_backingStore = stream.str();
   return *this;
}
