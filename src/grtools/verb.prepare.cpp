#include "../cmn/autoPtr.hpp"
#include "../cmn/error.hpp"
#include "../cmn/service.hpp"
#include "../console/arg.hpp"
#include "../console/log.hpp"
#include "../db/api.hpp"
#include "../file/api.hpp"
#include "../file/manager.hpp"
#include "../tcatlib/api.hpp"
#include <memory>

namespace {

class command : public console::iCommand {
public:
   std::string oPath;

protected:
   virtual void run(console::iLog& l);
};

class myVerb : public console::globalVerb {
public:
   virtual void dumpDocs(console::iLog& l)
   {
      l.writeLnInfo("--prepare <csv-file>");
      l.writeLnInfo("   modifies the CSV file for use with grtools");
   }

protected:
   virtual console::verbBase *inflate()
   {
      std::unique_ptr<console::verbBase> v(
         new console::verb<command>("--prepare"));

      v->addParameter(
         console::stringParameter::required(offsetof(command,oPath)));

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

   l.writeLnVerbose("build paths");
   std::string inPath = fMan->absolutize(oPath);
   l.writeLnVerbose("  inPath=<%s>",inPath.c_str());

   l.writeLnVerbose("loading CSV");
   tcat::typePtr<db::iFileManager> dbMan;
   std::unique_ptr<db::iFile> pCsvFile(&dbMan->load(inPath));

   l.writeLnVerbose("removing unmaintained shelf ordering column");
   pCsvFile->removeColumn("Bookshelves with positions");

   l.writeLnVerbose("saving CSV");
   dbMan->saveAs(*pCsvFile,inPath);
}

} // anonymous namespace
