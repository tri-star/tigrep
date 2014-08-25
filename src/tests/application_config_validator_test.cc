
#include "../application_config_validator.h"

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
      app_config.start_time = "2014-08-08 10:00:00";
      app_config.end_time   = "2014-08-10 10:00:00";
    }
    
    void fillWithValidInformationUsingLogType(ApplicationConfig& app_config) {
      app_config.log_type = "syslog";
      app_config.output_file = "/tmp/output.log";
      app_config.input_file = "/usr/bin/bash";
      app_config.start_time = "2014-08-08 10:00:00";
      app_config.end_time   = "2014-08-10 10:00:00";
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
  
  
  TEST_F(ApplicationConfigValidatorTest, with_all_parameters_filled_using_log_type) {
    ApplicationConfig app_config;
    fillWithValidInformationUsingLogType(app_config);

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

  TEST_F(ApplicationConfigValidatorTest, start_time_and_end_time_could_ommit) {
    ApplicationConfig app_config;
    fillWithValidInformation(app_config);
    app_config.start_time = "";
    app_config.end_time = "";
    
    ApplicationConfigValidator validator;
    
    bool is_ok = validator.validate(app_config);
    ASSERT_TRUE(is_ok);
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

  TEST_F(ApplicationConfigValidatorTest, with_invalid_start_time) {
    ApplicationConfig app_config;
    fillWithValidInformation(app_config);
    app_config.start_time = "2014-ab-08 10:00:00";
    
    ApplicationConfigValidator validator;
    
    bool is_ok = validator.validate(app_config);
    ASSERT_FALSE(is_ok);
    
    std::list<std::string> errors;
    validator.getErrors(&errors);
    assertMessageIncluded(errors, "invalid start date time.");
  }

  TEST_F(ApplicationConfigValidatorTest, with_invalid_end_time) {
    ApplicationConfig app_config;
    fillWithValidInformation(app_config);
    app_config.end_time = "2014-ab-08 10:00:00";
    
    ApplicationConfigValidator validator;
    
    bool is_ok = validator.validate(app_config);
    ASSERT_FALSE(is_ok);
    
    std::list<std::string> errors;
    validator.getErrors(&errors);
    assertMessageIncluded(errors, "invalid end date time.");
  }

} //namespace
