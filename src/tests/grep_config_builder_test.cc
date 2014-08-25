#include "../grep_config_builder.h"
#include "../application_config.h"
#include "../log_type_repository.h"
#include "../grep_command.h"

#include <gtest/gtest.h>

namespace tigrep {

  class GrepConfigBuilderTest : public ::testing::Test {

    // setup ApplicationConfig with regex and format specified.
    void setupValidAppConfigWithRegexAndFormat(ApplicationConfig* app_config) {
      app_config->regex_string = "regex_string";
      app_config->format = "%Y-%m-%d %H:%M:%S";
      app_config->start_time = "2014-08-08 10:00:00";
      app_config->end_time = "2014-08-08 14:00:00";

    }

    void setupLogTypeRepository(LogTypeRepository* log_type_repository) {

    }

  };


  TEST_F(GrepConfigBuilderTest, with_regex_and_format_specified) {

    ApplicationConfig app_config;
    LogTypeRepository log_type_repository;
    GrepConfigBuilder builder;
    GrepConfig grep_config;

    setupValidAppConfigWithRegexAndFormat(&app_config);
    setupLogTypeRepository(&log_type_repository);

    builder.build(app_config, log_type_repository, &grep_config);

    //Asserts
    //boost::regex pattern;
    //std::string format;
    //time_t start_date_time;
    //time_t end_date_time;
    //ist
    //ost


  }

} //namespace
