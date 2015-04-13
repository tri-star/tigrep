#include "../grep_command.h"
#include "test_lib/util.h"

#include <fstream>
#include <gtest/gtest.h>

namespace tigrep {
  

  class GrepCommandTest : public ::testing::Test {
    
  public:

    void SetUp() {
      TestUtil::getFixtureDirPath(&fixture_dir_);
    }

    std::string fixture_dir_;

  };

  //-----------------------------------------------------------------------
  //check that GrepCommand recognize valid start/end position.
  TEST_F(GrepCommandTest, basic) {

    std::istringstream ist(
        "abcdefg\n"
        "[2014-08-08 10:00:00] some-text1\n"
        "[2014-08-08 11:00:00] some-text2\n"
        "[2014-08-08 12:00:00] some-text3\n"
        "[2014-08-08 13:00:00] some-text4\n"
        "[2014-08-08 14:00:00] some-text5\n"
    );
    std::ostringstream ost;

    GrepConfig config;

    config.pattern = boost::regex("^\\[([0-9\\-:\\s]+?)\\]");
    config.format = "%Y-%m-%d %H:%M:%S";
    config.start_date_time = 1407463200;  //date -d "2014-08-08 11:00:00" +%s
    config.end_date_time = 1407470400;    //date -d "2014-08-08 13:00:00" +%s
    config.ist = &ist;
    config.ost = &ost;
    GrepCommand grep_command(config);

    grep_command.execute();


    std::istringstream ist_for_check(ost.str().c_str());
    char buffer[4096];
    memset(buffer, 0, 4096);

    //check grep_command.execute() result.
    //expect first line is "2014-08-08 11:00:00"
    ist_for_check.getline(buffer, 4096);
    ASSERT_STREQ("[2014-08-08 11:00:00] some-text2", buffer);

    //expect second line is "2014-08-08 12:00:00"
    ist_for_check.getline(buffer, 4096);
    ASSERT_STREQ("[2014-08-08 12:00:00] some-text3", buffer);

    //expect second line is "2014-08-08 13:00:00"
    ist_for_check.getline(buffer, 4096);
    ASSERT_STREQ("[2014-08-08 13:00:00] some-text4", buffer);

    //expect no lines left.
    ist_for_check.getline(buffer, 4096);
    ASSERT_STREQ("", buffer);
    ASSERT_TRUE(ist_for_check.eof());

  }

  //-----------------------------------------------------------------------
  //check that scan start from first line when "from" parameter was omitted.
  TEST_F(GrepCommandTest, omit_from_parameter) {

    std::istringstream ist(
        "abcdefg\n"
        "[2014-08-08 10:00:00] some-text1\n"
        "[2014-08-08 11:00:00] some-text2\n"
        "[2014-08-08 12:00:00] some-text3\n"
        "[2014-08-08 13:00:00] some-text4\n"
        "[2014-08-08 14:00:00] some-text5\n"
    );
    std::ostringstream ost;

    GrepConfig config;

    config.pattern = boost::regex("^\\[([0-9\\-:\\s]+?)\\]");
    config.format = "%Y-%m-%d %H:%M:%S";
    config.start_date_time = 0;  //0 means that parameter omitted.
    config.end_date_time = 1407470400;    //date -d "2014-08-08 13:00:00" +%s
    config.ist = &ist;
    config.ost = &ost;
    GrepCommand grep_command(config);

    grep_command.execute();


    std::istringstream ist_for_check(ost.str().c_str());
    char buffer[4096];
    memset(buffer, 0, 4096);

    //check grep_command.execute() result.
    //
    // When "from" parameter was omitted, the line that does not include a date part are skipped
    // until find the first line that include a date part.
    // This is current restriction.
    //
    ist_for_check.getline(buffer, 4096);
    ASSERT_STREQ("[2014-08-08 10:00:00] some-text1", buffer);

    //read next line.
    ist_for_check.getline(buffer, 4096);
    ASSERT_STREQ("[2014-08-08 11:00:00] some-text2", buffer);

    //read next line.
    ist_for_check.getline(buffer, 4096);
    ASSERT_STREQ("[2014-08-08 12:00:00] some-text3", buffer);

    //read next line.
    ist_for_check.getline(buffer, 4096);
    ASSERT_STREQ("[2014-08-08 13:00:00] some-text4", buffer);

    //expect no lines left.
    ist_for_check.getline(buffer, 4096);
    ASSERT_STREQ("", buffer);
    ASSERT_TRUE(ist_for_check.eof());
  }

  //-----------------------------------------------------------------------
  //check that scan reaches EOF when "to" parameter was omitted.
  TEST_F(GrepCommandTest, omit_to_parameter) {

    std::istringstream ist(
        "abcdefg\n"
        "[2014-08-08 10:00:00] some-text1\n"
        "[2014-08-08 11:00:00] some-text2\n"
        "[2014-08-08 12:00:00] some-text3\n"
        "[2014-08-08 13:00:00] some-text4\n"
        "[2014-08-08 14:00:00] some-text5\n"
    );
    std::ostringstream ost;

    GrepConfig config;

    config.pattern = boost::regex("^\\[([0-9\\-:\\s]+?)\\]");
    config.format = "%Y-%m-%d %H:%M:%S";
    config.start_date_time = 1407463200;  //date -d "2014-08-08 11:00:00" +%s
    config.end_date_time = 0;
    config.ist = &ist;
    config.ost = &ost;
    GrepCommand grep_command(config);

    grep_command.execute();


    std::istringstream ist_for_check(ost.str().c_str());
    char buffer[4096];
    memset(buffer, 0, 4096);

    //check grep_command.execute() result.
    ist_for_check.getline(buffer, 4096);
    ASSERT_STREQ("[2014-08-08 11:00:00] some-text2", buffer);

    ist_for_check.getline(buffer, 4096);
    ASSERT_STREQ("[2014-08-08 12:00:00] some-text3", buffer);

    ist_for_check.getline(buffer, 4096);
    ASSERT_STREQ("[2014-08-08 13:00:00] some-text4", buffer);

    ist_for_check.getline(buffer, 4096);
    ASSERT_STREQ("[2014-08-08 14:00:00] some-text5", buffer);

    //expect no lines left.
    ist_for_check.getline(buffer, 4096);
    ASSERT_STREQ("", buffer);
    ASSERT_TRUE(ist_for_check.eof());
  }

  //-----------------------------------------------------------------------
  //check that "apache" log type successfully recognized.
  TEST_F(GrepCommandTest, apache_log_type_test) {

    std::istringstream ist(
        "abcdefg\n"
        "100.101.102.103 - - [01/Jan/2015:00:00:00 +0900] \"GET /index.html HTTP/1.1\"\n"
        "100.101.102.103 - - [01/Jan/2015:00:10:00 +0900] \"GET /index.html HTTP/1.1\"\n"
        "100.101.102.103 - - [01/Jan/2015:00:20:00 +0900] \"GET /index.html HTTP/1.1\"\n"
        "100.101.102.103 - - [01/Jan/2015:00:30:00 +0900] \"GET /index.html HTTP/1.1\"\n"
    );
    std::ostringstream ost;

    GrepConfig config;

    //TODO: get pattern, format from LogTypeRepository.
    config.pattern = "^[^ ]+? [^ ]+? [^ ]+? \\[([^\\]]+?)\\]";
    config.format = "%d/%b/%Y:%H:%M:%S %z";
    config.start_date_time = 1420038900;  //date -d "2015-01-01 00:15:00" +%s
    config.end_date_time = 1420039500;  //date -d "2015-01-01 00:25:00" +%s
    config.ist = &ist;
    config.ost = &ost;
    GrepCommand grep_command(config);

    grep_command.execute();


    std::istringstream ist_for_check(ost.str().c_str());
    char buffer[4096];
    memset(buffer, 0, 4096);

    //check grep_command.execute() result.
    ist_for_check.getline(buffer, 4096);
    ASSERT_STREQ("100.101.102.103 - - [01/Jan/2015:00:20:00 +0900] \"GET /index.html HTTP/1.1\"", buffer);

    //expect no lines left.
    ist_for_check.getline(buffer, 4096);
    ASSERT_STREQ("", buffer);
    ASSERT_TRUE(ist_for_check.eof());
  }


  TEST_F(GrepCommandTest, binary_search_test) {

    std::string fixture_path(fixture_dir_);

    fixture_path.append("/binary_search_test.log");

    std::ifstream ist;
    std::ostringstream ost;
    ist.open(fixture_path.c_str());
    if(ist.fail()) {
      throw std::runtime_error("Failed to open input file.");
    }

    GrepConfig config;

    //TODO: get pattern, format from LogTypeRepository.
    config.pattern = boost::regex("^\\[([0-9\\-:\\s]+?)\\]");
    config.format = "%Y-%m-%d %H:%M:%S";
    config.start_date_time = 1407460800;  //date -d "2014-08-08 10:20:00" +%s
    config.end_date_time = 1407460860;  //date -d "2014-08-08 10:21:00" +%s
    config.input_file = fixture_path;
    config.ist = &ist;
    config.ost = &ost;
    GrepCommand grep_command(config);

    grep_command.execute();


    std::istringstream ist_for_check(ost.str().c_str());
    char buffer[4096];
    memset(buffer, 0, 4096);

    //check grep_command.execute() result.
    ist_for_check.getline(buffer, 4096);
    ASSERT_STREQ("[2014-08-08 10:20:00] some-text20", buffer);

  }


  TEST_F(GrepCommandTest, small_binary_search_test) {

    std::string fixture_path(fixture_dir_);

    fixture_path.append("/binary_search_small.log");

    std::ifstream ist;
    std::ostringstream ost;
    ist.open(fixture_path.c_str());
    if(ist.fail()) {
      throw std::runtime_error("Failed to open input file.");
    }

    GrepConfig config;

    //TODO: get pattern, format from LogTypeRepository.
    config.pattern = boost::regex("^\\[([0-9\\-:\\s]+?)\\]");
    config.format = "%Y-%m-%d %H:%M:%S";
    config.start_date_time = 1407459720;  //date -d "2014-08-08 10:02:00" +%s
    config.end_date_time = 1407459721; //date -d "2014-08-08 10:02:00" +%s
    config.input_file = fixture_path;
    config.ist = &ist;
    config.ost = &ost;
    GrepCommand grep_command(config);

    grep_command.execute();


    std::istringstream ist_for_check(ost.str().c_str());
    char buffer[4096];
    memset(buffer, 0, 4096);

    //check grep_command.execute() result.
    ist_for_check.getline(buffer, 4096);
    ASSERT_STREQ("[2014-08-08 10:02:00] some-text2", buffer);

  }
} //namespace
