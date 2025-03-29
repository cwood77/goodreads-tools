#ifndef ___db_api___
#define ___db_api___

#include <list>
#include <string>

namespace db {

   // TODO - cut/move comments
// operations I need
// - remove column
// - split, sort, and reform field
// - load
// - save, using other database item ordering
// - demand column
// - split field, and demand (or make-up) tag
// - split field, set tag, reform
// - merge databases

// --------------------------------------------------------------------------------------

class listField {
public:
   explicit listField(std::wstring& s);
   std::list<std::wstring> filter(const std::wstring& prefix, const std::wstring& def) const;
   listField& add(const std::wstring& value);
   listField& erase(const std::wstring& value);
   listField& sort();
   listField& save();
};

class iItem {
public:
   virtual ~iItem() {}
   virtual std::wstring& demandValue(const std::wstring& key) = 0;
};

class iFile {
public:
   virtual ~iFile() {}
   //virtual void foreachItem(iItem& i) = 0;
   //virtual void removeColumn(const std::wstring& key) = 0;
};

class iFileManager {
public:
   virtual ~iFileManager() {}
   virtual iFile& load(const std::string& path) const = 0;
   virtual void mergeInto(iFile& dest, const iFile& source) const = 0;
   virtual void saveAs(const iFile& f, const std::string& path) const = 0;
};

} // namespace db

#endif // ___db_api___
