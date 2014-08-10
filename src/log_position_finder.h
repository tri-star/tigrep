#ifndef TIGREP_LOG_POSITION_FINDER_H_
#define TIGREP_LOG_POSITION_FINDER_H_

#include "util/date_compare.h"

#include <boost/regex.hpp>
#include <istream>
#include <string>
#include <ctime>

namespace tigrep {
  
  class LogPositionFinder {
    
  private:
    std::istream* ist_;
    boost::regex pattern_;
    std::string format_;
    unsigned long long current_line_;
    
  public:
    
    LogPositionFinder(std::istream& ist, boost::regex pattern, std::string format) {
      initialize(ist, pattern, format);
    }
    
    void initialize(std::istream& ist, boost::regex pattern, std::string format) {
      ist_ = &ist;
      pattern_ = pattern;
      format_ = format;
      current_line_ = 0;
    }
    
    unsigned long long getStartLine(time_t start_time) {
      const int buffer_size = 4096;
      char* buffer = new char[buffer_size];
      std::string date_string_buffer;
      util::DateCompare date_compare;
      time_t log_date;
      boost::cmatch matches;
      while(!ist_->eof()) {
        ist_->getline(buffer, buffer_size);
        current_line_++;
        
        if(ist_->bad()) {
          throw std::runtime_error("file read error.");
        }
        
        if(!boost::regex_search(buffer, matches, pattern_)) {
          continue;
        }
        
        date_string_buffer = matches.str(1);
        date_compare.setDateTime(start_time);
        if(date_compare.compare(date_string_buffer.c_str(), format_.c_str(), util::DateCompare::kIS_GTE)) {
          return current_line_;
        }
      }
      delete[] buffer;
      
      return 0;
    }
    
    virtual ~LogPositionFinder() {
    }
    
  };
  
} // namespace

#endif
