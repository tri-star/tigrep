#include <gtest/gtest.h>

#include "../src/log_position_finder.h"

#include <sstream>

namespace tigrep {

  TEST(test_test, test_01) {
    ASSERT_EQ(2, 2);
  }

  TEST(log_position_finder, basic) {
    
    std::istringstream string_stream("abcdefg\n<2014-08-10 10:00:00>abcdefg");
    boost::regex pattern("^<([0-9\\-\\s:]+?)>");
    std::string format("%Y-%m-%d %H:%M:%S");
    
    LogPositionFinder log_position_finder(string_stream, pattern, format);
    time_t start_date_time = 1407632400;
    unsigned long long start_line = log_position_finder.getStartLine(start_date_time);
    
    ASSERT_EQ(2, start_line);
  }
  
} //namespace
