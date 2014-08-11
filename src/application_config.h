#ifndef TIGREP_APPLICATION_CONFIG_H_
#define TIGREP_APPLICATION_CONFIG_H_

#include <string>

namespace tigrep {
  
  class ApplicationConfig {
    
  public:
    std::string regex_string;
    std::string format;
    std::string output_file;
    std::string input_file;
    
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
    
  };
  
} //namespace

#endif
