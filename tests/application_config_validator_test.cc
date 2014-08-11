
#include "../src/application_config_validator.h"

#include <list>
#include <string>
#include <gtest/gtest.h>

namespace tigrep {

  class ApplicationConfigValidatorTest: public ::testing::Test {
    
  protected:
    
    void fillWithValidInformation(ApplicationConfig& app_config) {
      app_config.regex_string = "some_expression";
      app_config.format = "%Y-%m-%d";
      app_config.output_file = "/tmp/output.log";
      app_config.input_file = "/usr/bin/bash";
    }
    
    void assertMessageIncluded(std::list<std::string>& errors, char* message) {
      std::list<std::string>::iterator i;
      
      bool found = false;
      for(i = errors.begin(); i != errors.end(); i++) {
        if(strcmp((*i).c_str(), message) == 0) {
          found = true;
        }
      }
      ASSERT_TRUE(found);
    }
  };
  
  
  
  
  TEST_F(ApplicationConfigValidatorTest, with_all_parameters_filled) {
    ApplicationConfig app_config;
    fillWithValidInformation(app_config);
    
    ApplicationConfigValidator validator;
    
    bool is_ok = validator.validate(app_config);
    
    ASSERT_TRUE(is_ok);
  }
  
  
  TEST_F(ApplicationConfigValidatorTest, with_regex_missing) {
    ApplicationConfig app_config;
    fillWithValidInformation(app_config);
    app_config.regex_string = "";
    
    ApplicationConfigValidator validator;
    
    bool is_ok = validator.validate(app_config);
    ASSERT_FALSE(is_ok);
    
    std::list<std::string> errors;
    validator.getErrors(&errors);
    assertMessageIncluded(errors, "regex option is required.");
  }
  
  TEST_F(ApplicationConfigValidatorTest, with_format_missing) {
    ApplicationConfig app_config;
    fillWithValidInformation(app_config);
    app_config.format = "";
    
    ApplicationConfigValidator validator;
    
    bool is_ok = validator.validate(app_config);
    ASSERT_FALSE(is_ok);
    
    std::list<std::string> errors;
    validator.getErrors(&errors);
    assertMessageIncluded(errors, "format option is required.");
  }

  TEST_F(ApplicationConfigValidatorTest, with_input_file_is_not_exist) {
    ApplicationConfig app_config;
    fillWithValidInformation(app_config);
    app_config.input_file = "./unknown_file";
    
    ApplicationConfigValidator validator;
    
    bool is_ok = validator.validate(app_config);
    ASSERT_FALSE(is_ok);
    
    std::list<std::string> errors;
    validator.getErrors(&errors);
    assertMessageIncluded(errors, "input file not found.");
  }

} //namespace
