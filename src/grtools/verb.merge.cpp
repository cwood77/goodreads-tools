#include "../cmn/autoPtr.hpp"
#include "../cmn/service.hpp"
#include "../console/arg.hpp"
#include "../console/log.hpp"
#include "../db/api.hpp"
#include "../file/api.hpp"
#include "../file/manager.hpp"
#include "../survey/api.hpp"
#include "../tcatlib/api.hpp"
#include <memory>

namespace {

class command : public console::iCommand {
public:
   std::string oCsvPath;
   std::string oOutFolder;

protected:
   virtual void run(console::iLog& l);
};

class myVerb : public console::globalVerb {
public:
   virtual void dumpDocs(console::iLog& l)
   {
      l.writeLnInfo("--merge <csv-file> <out-folder>");
      l.writeLnInfo("   combine exploded folder into a CSV file");
   }

protected:
   virtual console::verbBase *inflate()
   {
      std::unique_ptr<console::verbBase> v(
         new console::verb<command>("--merge"));

      v->addParameter(
         console::stringParameter::required(offsetof(command,oCsvPath)));
      v->addParameter(
         console::stringParameter::required(offsetof(command,oOutFolder)));

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

   l.writeLnVerbose("process parameters");
   std::string inPath = fMan->absolutize(oCsvPath);
   std::string outPath = fMan->absolutize(oOutFolder);
   l.writeLnVerbose("     inPath=<%s>",inPath.c_str());
   l.writeLnVerbose("    outPath=<%s>",outPath.c_str());

   l.writeLnVerbose("loading CSV");
   tcat::typePtr<db::iFileManager> dbMan;
   std::unique_ptr<db::iFile> pCsvFile(&dbMan->load(inPath));

   l.writeLnVerbose("finding books");
   tcat::typePtr<survey::iScanner> scanner;
   std::unique_ptr<survey::iCatalog> pCatalog(&scanner->scan(outPath));
   pCatalog->foreach([&](const std::string& tagName, const std::string& fileName)
   {
      console::autoIndent _i2(l);

      l.writeLnVerbose("book: %s",fileName.c_str());
      console::autoIndent _i(l);

      l.writeLnVerbose("loading");
      std::unique_ptr<db::iFile> pBookFile(
         &dbMan->load(outPath + "\\" + tagName + "\\" + fileName));

      l.writeLnVerbose("setting tag");
      pBookFile->foreach([&](auto& i)
      {
         db::listField(i.demand("Bookshelves"))
            .add(tagName)
            .save();
      });

      l.writeLnVerbose("merging");
      dbMan->updateFrom(*pCsvFile,*pBookFile);
   });
}

} // anonymous namespace
