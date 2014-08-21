#ifndef TIGREP_APPLICATION_CONFIG_H_
#define TIGREP_APPLICATION_CONFIG_H_

#include <string>

namespace tigrep {
  
  class ApplicationConfig {
    
  public:
    std::string log_type;
    std::string regex_string;
    std::string format;
    std::string output_file;
    std::string input_file;
    std::string start_time;
    std::string end_time;
    
    ApplicationConfig() {
    }
    
    virtual ~ApplicationConfig() {
    }
    
    bool isOutputFileSpecified() {
      return output_file.length() == 0 ? false : true;
    }
    
    bool isInputFileSpecified() {
      return input_file.length() == 0 ? false : true;
    }
    
    time_t getStartTime() {
      struct tm tm;
      char* result = strptime(start_time.c_str(), format.c_str(), &tm);
      if(result == 0) {
        return 0;
      }
      return mktime(&tm);
    }
    
    time_t getEndTime() {
      struct tm tm;
      char* result = strptime(end_time.c_str(), format.c_str(), &tm);
      if(result == 0) {
        return 0;
      }
      return mktime(&tm);
    }
  };
  
} //namespace

#endif
