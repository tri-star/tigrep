#ifndef LOG_TYPE_REPOSITORY_H_
#define LOG_TYPE_REPOSITORY_H_

#include <map>
#include <string>


namespace tigrep {

  typedef struct LogTypeEntry {
    std::string name;
    std::string regex;
    std::string format;
	} LogTypeEntry;

	typedef std::map<std::string, LogTypeEntry*> LogTypeMap;


  /**
   * A class that holds "log type" definitions.
   */
  class LogTypeRepository {

  public:

	  LogTypeRepository() {
	  }

	  virtual ~LogTypeRepository() {
	  }

	  void addEntry(LogTypeEntry& entry) {
	    entries_[entry.name] = &entry;
	  }

	  const LogTypeMap& getList() {
	    return entries_;
	  }

	  LogTypeEntry* getEntry(const std::string& name) {
	    return getEntry(name.c_str());
	  }

    LogTypeEntry* getEntry(const char* name) {
      LogTypeMap::iterator i = entries_.find(name);
      if(i == entries_.end()) {
        return NULL;
      }
      return i->second;
    }
  private:

	  LogTypeMap entries_;

  };

} //name space

#endif
