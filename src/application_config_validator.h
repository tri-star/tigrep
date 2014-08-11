#ifndef TIGREP_APPLICATION_CONFIG_VALIDATOR_H_
#define TIGREP_APPLICATION_CONFIG_VALIDATOR_H_

#include "application_config.h"

#include <list>
#include <string>
#include <ctime>
#include <string.h>
#include <sys/stat.h>


namespace tigrep {
  
  class ApplicationConfigValidator {
    
  public:
    
    ApplicationConfigValidator() {
      is_error_ = false;
    }
    
    virtual ~ApplicationConfigValidator() {
    }
    
    bool validate(ApplicationConfig& app_config) {
      
      if(app_config.regex_string.length() == 0) {
        errors_.push_back("regex option is required.");
      }
      if(app_config.format.length() == 0) {
        errors_.push_back("format option is required.");
      }
      
      if(app_config.input_file.length() != 0) {
        if(!isFile(app_config.input_file.c_str())) {
          errors_.push_back("input file not found.");
        }
      }
      
      return (errors_.size() == 0);
    }
    
    bool isError() {
      return is_error_;
    }
    
    /**
     * get error messages while validating config.
     * @var errors empty container to store error messages.
     */
    void getErrors(std::list<std::string>* errors) {
      *errors = errors_;
    }
    
  private:
    bool is_error_;
    std::list<std::string> errors_;
    
    bool isFile(const char* path) {
      struct stat st;
      int stat_result = stat(path, &st);
      
      if(stat_result != 0) {
        return false;
      }
      return (S_ISREG(st.st_mode));
    }
    
  };
  
}

#endif
