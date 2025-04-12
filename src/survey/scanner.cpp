#define WIN32_LEAN_AND_MEAN
#include "../cmn/error.hpp"
#include "../cmn/win32.hpp"
#include "../tcatlib/api.hpp"
#include "api.hpp"
#include <memory>
#include <utility>
#include <windows.h>

namespace survey {
namespace {

class catalog : public iCatalog {
public:
   virtual void foreach(std::function<void(const std::string&,const std::string&)> f)
   { for(auto& e : m_entries) f(e.first,e.second); }

   void addFile(const std::string& tagName, const std::string& fileName)
   { m_entries.push_back(std::make_pair(tagName,fileName)); }

private:
   std::list<std::pair<std::string,std::string> > m_entries;
};

class catalogWriter {
public:
   explicit catalogWriter(catalog& c) : m_c(c) {}
   void setDirectory(const std::string& d) { m_d = d; }
   void addFileName(const std::string& fn) { m_c.addFile(m_d,fn); }

private:
   catalog& m_c;
   std::string m_d;
};

class fileScanner {
public:
   static void scan(const std::string& basePath, catalogWriter& w)
   {
      WIN32_FIND_DATA fData;
      cmn::autoFindHandle hFind;
      hFind.h = ::FindFirstFileA((basePath + "\\*.csv").c_str(),&fData);
      if(hFind.h == INVALID_HANDLE_VALUE)
         cmn::error(cdwHere,"bad path")
            .with("path",basePath)
            .raise();
      do
      {
         if(std::string(".") == fData.cFileName)
            continue;
         if(std::string("..") == fData.cFileName)
            continue;

         if(!(fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            w.addFileName(fData.cFileName);

      } while(::FindNextFileA(hFind.h,&fData));
   }
};

class dirScanner {
public:
   static void scan(const std::string& basePath, catalogWriter& w)
   {
      WIN32_FIND_DATA fData;
      cmn::autoFindHandle hFind;
      hFind.h = ::FindFirstFileA((basePath + "\\*").c_str(),&fData);
      if(hFind.h == INVALID_HANDLE_VALUE)
         cmn::error(cdwHere,"bad path")
            .with("path",basePath)
            .raise();
      do
      {
         if(std::string(".") == fData.cFileName)
            continue;
         if(std::string("..") == fData.cFileName)
            continue;

         if(fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
         {
            w.setDirectory(fData.cFileName);
            fileScanner::scan(basePath + "\\" + fData.cFileName,w);
         }

      } while(::FindNextFileA(hFind.h,&fData));
   }
};

class scanner : public iScanner {
public:
   virtual iCatalog& scan(const std::string& rootPath)
   {
      std::unique_ptr<catalog> pCat(new catalog());
      catalogWriter w(*pCat);
      dirScanner::scan(rootPath,w);
      return *pCat.release();
   }
};

tcatExposeTypeAs(scanner,iScanner);

} // anonymous namespace
} // namespace survey
