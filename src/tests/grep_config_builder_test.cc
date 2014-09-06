#include "../grep_config_builder.h"
#include "../application_config.h"
#include "../log_type_repository.h"
#include "../grep_command.h"

#include <string.h>
#include <unistd.h>
#include <libgen.h>

#include <gtest/gtest.h>

namespace tigrep {

  class GrepConfigBuilderTest : public ::testing::Test {

  public:

    virtual void SetUp() {
      getFixtureDirPath(&fixture_dir_);
    }

    // setup ApplicationConfig with regex and format specified.
    void setupValidAppConfigWithRegexAndFormat(ApplicationConfig* app_config) {

      app_config->regex_string = "regex_string";
      app_config->format = "%Y-%m-%d %H:%M:%S";
      app_config->start_time = "2014-08-08 10:00:00";
      app_config->end_time = "2014-08-08 14:00:00";
      app_config->input_file = fixture_dir_;
      app_config->input_file.append("/empty.log");
      app_config->output_file = fixture_dir_;
      app_config->output_file.append("/output_test.log");

    }

    void setupValidAppConfigWithLogType(ApplicationConfig* app_config) {
      app_config->log_type = "mysql-general";
      app_config->start_time = "140808 10:00:00";
      app_config->end_time = "140808 14:00:00";
    }

    void setupLogTypeRepository(LogTypeRepository* log_type_repository) {
      LogTypeEntry* entries[2];
      entries[0] = new LogTypeEntry();
      entries[0]->name = "mysql-general";
      entries[0]->regex = "regex";
      entries[0]->format = "%y%m%d %H:%M:%S";

      entries[1] = new LogTypeEntry();
      entries[1]->name = "test-log";
      entries[1]->regex = "regex2";
      entries[1]->format = "%Y-%m-%d %H:%M:%S";
      log_type_repository->addEntry(*entries[0]);
      log_type_repository->addEntry(*entries[1]);
    }

  private:

    std::string fixture_dir_;

    void getFixtureDirPath(std::string* fixture_dir) {
      std::string bin_dir;

      getBinDir(&bin_dir);

      *fixture_dir = bin_dir;
      fixture_dir->append("/../src/tests/fixtures");
    }

    void getBinDir(std::string* bin_dir_string) {
      char proc_exe_path[256];
      char bin_path[256];
      memset(proc_exe_path, 0, 256);
      memset(bin_path, 0, 256);

      sprintf(proc_exe_path, "/proc/%d/exe", getpid());
      readlink(proc_exe_path, bin_path, 255);

      dirname(bin_path);
      *bin_dir_string = bin_path;
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
    ASSERT_STREQ("regex_string", grep_config.pattern.str().c_str());
    ASSERT_STREQ("%Y-%m-%d %H:%M:%S", grep_config.format.c_str());
    //date -d "2014-08-08 10:00:00" +%s
    ASSERT_EQ(1407459600, grep_config.start_date_time);
    //date -d "2014-08-08 14:00:00" +%s
    ASSERT_EQ(1407474000, grep_config.end_date_time);

    ASSERT_TRUE(&std::cin  != grep_config.ist);
    ASSERT_TRUE(&std::cout != grep_config.ost);
  }

  TEST_F(GrepConfigBuilderTest, with_log_type_specified) {

    ApplicationConfig app_config;
    LogTypeRepository log_type_repository;
    GrepConfigBuilder builder;
    GrepConfig grep_config;

    setupValidAppConfigWithLogType(&app_config);
    setupLogTypeRepository(&log_type_repository);

    builder.build(app_config, log_type_repository, &grep_config);

    //Asserts
    ASSERT_STREQ("regex", grep_config.pattern.str().c_str());
    ASSERT_STREQ("%y%m%d %H:%M:%S", grep_config.format.c_str());
    //date -d "2014-08-08 10:00:00" +%s
    ASSERT_EQ(1407459600, grep_config.start_date_time);
    //date -d "2014-08-08 14:00:00" +%s
    ASSERT_EQ(1407474000, grep_config.end_date_time);

    ASSERT_TRUE(&std::cin  == grep_config.ist);
    ASSERT_TRUE(&std::cout == grep_config.ost);
  }

  TEST_F(GrepConfigBuilderTest, with_start_time_omitted) {

    ApplicationConfig app_config;
    LogTypeRepository log_type_repository;
    GrepConfigBuilder builder;
    GrepConfig grep_config;

    setupValidAppConfigWithLogType(&app_config);
    setupLogTypeRepository(&log_type_repository);

    app_config.start_time = "";

    builder.build(app_config, log_type_repository, &grep_config);

    //Asserts
    ASSERT_EQ(0, grep_config.start_date_time);
  }

  TEST_F(GrepConfigBuilderTest, with_end_time_omitted) {

    ApplicationConfig app_config;
    LogTypeRepository log_type_repository;
    GrepConfigBuilder builder;
    GrepConfig grep_config;

    setupValidAppConfigWithLogType(&app_config);
    setupLogTypeRepository(&log_type_repository);

    app_config.end_time = "";

    builder.build(app_config, log_type_repository, &grep_config);

    //Asserts
    ASSERT_EQ(0, grep_config.end_date_time);
  }

  TEST_F(GrepConfigBuilderTest, with_invalid_start_time) {

    ApplicationConfig app_config;
    LogTypeRepository log_type_repository;
    GrepConfigBuilder builder;
    GrepConfig grep_config;

    setupValidAppConfigWithLogType(&app_config);
    setupLogTypeRepository(&log_type_repository);

    app_config.start_time = "1234abcd";

    builder.build(app_config, log_type_repository, &grep_config);

    //Asserts
    ASSERT_EQ(0, grep_config.start_date_time);
  }


  TEST_F(GrepConfigBuilderTest, with_invalid_end_time) {

    ApplicationConfig app_config;
    LogTypeRepository log_type_repository;
    GrepConfigBuilder builder;
    GrepConfig grep_config;

    setupValidAppConfigWithLogType(&app_config);
    setupLogTypeRepository(&log_type_repository);

    app_config.end_time = "abcd";  //empty

    builder.build(app_config, log_type_repository, &grep_config);

    //Asserts
    ASSERT_EQ(0, grep_config.end_date_time);
  }

} //namespace
