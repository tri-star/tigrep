#include "../../util/date_util.h"

#include <string>

#include <gtest/gtest.h>


namespace tigrep {

  namespace util {

    TEST(DateUtilTest, basic) {
      std::string time = "2014-08-08 10:00:00";
      std::string format = "%Y-%m-%d %H:%M:%S";
      time_t expected = 1407459600; //date -d "2014-08-08 10:00:00" +%s
      time_t actual = DateUtil::stringToTime(time, format);

      ASSERT_EQ(expected, actual);
    }

    TEST(DateUtilTest, with_empty_date) {
      std::string time = "";
      std::string format = "%Y-%m-%d %H:%M:%S";
      time_t expected = 0;
      time_t actual = DateUtil::stringToTime(time, format);

      ASSERT_EQ(expected, actual);
    }

    TEST(DateUtilTest, with_invalid_date) {
      std::string time = "abcdefg";
      std::string format = "%Y-%m-%d %H:%M:%S";
      time_t expected = 0;
      time_t actual = DateUtil::stringToTime(time, format);

      ASSERT_EQ(expected, actual);
    }

    TEST(DateUtilTest, with_incomplete_date) {
      std::string time = "2014-08-08 10:00:0a";
      std::string format = "%Y-%m-%d %H:%M:%S";
      time_t expected = 1407459600; //date -d "2014-08-08 10:00:00" +%s
      time_t actual = DateUtil::stringToTime(time, format);

      ASSERT_EQ(expected, actual);
    }
  } //namespace
} //namespace
