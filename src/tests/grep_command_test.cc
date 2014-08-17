#include "../grep_command.h"

#include <gtest/gtest.h>

namespace tigrep {
  

//check that GrepCommand recognize valid start/end position.
  TEST(grep_command, basic) {
    
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
    GrepCommand grep_command(&ist, &ost, config);

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
} //namespace
