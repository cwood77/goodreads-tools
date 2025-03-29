#ifndef ___db_api___
#define ___db_api___

#include <functional>
#include <list>
#include <set>
#include <sstream>
#include <string>

namespace db {

class iFile;

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

// class iTagFilter {
// public:
//    virtual void onTagsStart() = 0;
//    virtual void onTag(const std::string& t) = 0;
//    virtual void onTagsDone() = 0;
// };

class listField {
public:
   explicit listField(std::string& s);

   listField& foreach(std::function<void(const std::string&)> f);
   //std::list<std::string> filter(const std::string& prefix, const std::string& def) const;
   //listField& add(const std::string& value);
   listField& erase(const std::string& value);

   listField& sort();
   listField& save();

private:
   std::string& m_backingStore;
   std::set<std::string> m_values;
};

class iItem {
public:
   virtual ~iItem() {}
   virtual std::string& demand(const std::string& key) = 0;
   virtual const std::string& demand(const std::string& key) const = 0;
   virtual iFile& cloneNewFile() const = 0;
};

class iFile {
public:
   virtual ~iFile() {}
   virtual void removeColumn(const std::string& key) = 0;
   virtual void foreach(std::function<void(iItem&)> f) = 0;
};

class iFileManager {
public:
   virtual ~iFileManager() {}
   virtual iFile& load(const std::string& path) const = 0;
   virtual void mergeInto(iFile& dest, const iFile& source) const = 0;
   virtual void saveAs(const iFile& f, const std::string& path) const = 0;
};

#include "api.ipp"

} // namespace db

#endif // ___db_api___
