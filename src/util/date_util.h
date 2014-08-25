#ifndef TIGREP_UTIL_DATE_UTIL_H_
#define TIGREP_UTIL_DATE_UTIL_H_

#include <string>
#include <ctime>

namespace tigrep {

  namespace util {

    class DateUtil {

    public:

      static time_t stringToTime(const std::string& time, const std::string& format) {
        struct tm tm;
        char* result = strptime(time.c_str(), format.c_str(), &tm);
        if(result == time.c_str() || result == 0) {
          return 0;
        }
        return mktime(&tm);
      }

    };

  } //namespace
} //namespace


#endif
