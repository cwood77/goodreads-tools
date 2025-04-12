#include "../cmn/autoPtr.hpp"
#include "../cmn/error.hpp"
#include "../cmn/service.hpp"
#include "../console/arg.hpp"
#include "../console/log.hpp"
#include "../db/api.hpp"
#include "../file/api.hpp"
#include "../file/manager.hpp"
#include "../tag/api.hpp"
#include "../tcatlib/api.hpp"
#include "../title/api.hpp"
#include <memory>

namespace {

class command : public console::iCommand {
public:
   std::string oCsvPath;
   std::string oOutFolder;
   std::string oTagSyntax;
   std::string oTitleSyntax;

protected:
   virtual void run(console::iLog& l);
};

class myVerb : public console::globalVerb {
public:
   virtual void dumpDocs(console::iLog& l)
   {
      l.writeLnInfo("--split <csv-file> <out-folder> <tag-syntax> <title-syntax>");
      l.writeLnInfo("   explodes a prepared CSV file into many files under a folder");
   }

protected:
   virtual console::verbBase *inflate()
   {
      std::unique_ptr<console::verbBase> v(
         new console::verb<command>("--split"));

      v->addParameter(
         console::stringParameter::required(offsetof(command,oCsvPath)));
      v->addParameter(
         console::stringParameter::required(offsetof(command,oOutFolder)));
      v->addParameter(
         console::stringParameter::optional(offsetof(command,oTagSyntax)));
      v->addParameter(
         console::stringParameter::optional(offsetof(command,oTitleSyntax)));

      return v.release();
   }
} gVerb;

void command::run(console::iLog& l)
{
   l.writeLnDebug("loading config settings (optional)");
   tcat::typePtr<file::iFileManager> fMan;
   cmn::autoReleasePtr<file::iSstFile> pFile(&fMan->bindFile<file::iSstFile>(
      file::iFileManager::kExeAdjacent,
      "config.sst",
      file::iFileManager::kDiscardOnClose
   ));
   pFile->tie(l);
   l.configure(pFile->dict());

   l.writeLnVerbose("compiling services");
   tcat::typePtr<cmn::serviceManager> svcMan;
   cmn::autoService<console::iLog>  _svc_l(*svcMan,l);
   cmn::autoService<sst::dict>      _svc_rc(*svcMan,pFile->dict(),"raw-config");

   l.writeLnVerbose("creating managers");
   tcat::typePtr<tag::iManager> tagMan;
   tcat::typePtr<title::iManager> titleMan;

   l.writeLnVerbose("process parameters");
   std::string inPath = fMan->absolutize(oCsvPath);
   std::string outPath = fMan->absolutize(oOutFolder);
   std::string tagSyntax = oTagSyntax.empty() ? tagMan->getDefault() : oTagSyntax;
   std::string titleSyntax = oTitleSyntax.empty() ? titleMan->getDefault() : oTitleSyntax;
   l.writeLnVerbose("     inPath=<%s>",inPath.c_str());
   l.writeLnVerbose("    outPath=<%s>",outPath.c_str());
   l.writeLnVerbose("  tagSyntax=<%s>",tagSyntax.c_str());
   l.writeLnVerbose("titleSyntax=<%s>",titleSyntax.c_str());

   l.writeLnVerbose("creating experts");
   std::unique_ptr<tag::iExpert> pTagEx(&tagMan->createExpert(tagSyntax));
   std::unique_ptr<title::iExpert> pTitleEx(&titleMan->createExpert(titleSyntax));

   l.writeLnVerbose("loading CSV");
   tcat::typePtr<db::iFileManager> dbMan;
   std::unique_ptr<db::iFile> pCsvFile(&dbMan->load(inPath));

   l.writeLnVerbose("handling each book");
   {
      console::autoIndent _i(l);
      pCsvFile->foreach([&](auto& i)
      {
         std::string fileName = i.demand("Title") + ".csv";

         db::listField tags(i.demand("Bookshelves"));
         std::unique_ptr<tag::iSet> pTags(&pTagEx->removeTags(tags));
         pTags->foreach([&](auto& t)
         {
            std::string fullPath = outPath + "\\" + t + "\\" + fileName;
            l.writeLnVerbose("writing book %s",fullPath.c_str());
            std::unique_ptr<db::iFile> pNewFile(&i.cloneNewFile());
            fMan->createAllFoldersForFile(fullPath.c_str(),l,/*really*/true);
            dbMan->saveAs(*pNewFile,fullPath);
         });
      });
   }
}

} // anonymous namespace
