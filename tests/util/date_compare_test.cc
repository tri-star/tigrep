#include <gtest/gtest.h>

#include "../../src/util/date_compare.h"

#include <sstream>

namespace tigrep {
  namespace util {
  
    TEST(date_compare, initialize_with_timestamp) {
      //date -d "2014-08-10 10:00:00" +%s
      time_t base_time = 1407632400;
      DateCompare d(base_time);
      
      ASSERT_TRUE(d.compare(base_time + 1, DateCompare::kIS_GT));
      ASSERT_FALSE(d.compare(base_time, DateCompare::kIS_GT));
      
    }
    
    TEST(date_compare, initialize_with_text) {
      //date -d "2014-08-10 10:00:00" +%s
      time_t base_time = 1407632400;
      DateCompare d(base_time);
      
      ASSERT_TRUE(d.compare(base_time + 1, DateCompare::kIS_GT));
      ASSERT_FALSE(d.compare(base_time, DateCompare::kIS_GT));
      
    }
    
    TEST(date_compare, compare_with_timestamp) {
      
      //date -d "2014-08-10 10:00:00" +%s
      time_t base_time = 1407632400;
      DateCompare d(base_time);
      
      ASSERT_TRUE(d.compare(base_time + 1, DateCompare::kIS_GT));
      ASSERT_FALSE(d.compare(base_time, DateCompare::kIS_GT));
      ASSERT_FALSE(d.compare(base_time - 1, DateCompare::kIS_GT));
      
      ASSERT_TRUE(d.compare(base_time + 1, DateCompare::kIS_GTE));
      ASSERT_TRUE(d.compare(base_time, DateCompare::kIS_GTE));
      ASSERT_FALSE(d.compare(base_time - 1, DateCompare::kIS_GTE));
      
      ASSERT_TRUE(d.compare(base_time, DateCompare::kIS_EQ));
      ASSERT_FALSE(d.compare(base_time + 1, DateCompare::kIS_EQ));
      ASSERT_FALSE(d.compare(base_time - 1, DateCompare::kIS_EQ));
      
      ASSERT_TRUE(d.compare(base_time - 1, DateCompare::kIS_LT));
      ASSERT_FALSE(d.compare(base_time, DateCompare::kIS_LT));
      ASSERT_FALSE(d.compare(base_time + 1, DateCompare::kIS_LT));
      
      ASSERT_TRUE(d.compare(base_time - 1, DateCompare::kIS_LTE));
      ASSERT_TRUE(d.compare(base_time, DateCompare::kIS_LTE));
      ASSERT_FALSE(d.compare(base_time + 1, DateCompare::kIS_LTE));
    }

    TEST(date_compare, compare_with_text) {
      
      //date -d "2014-08-10 10:00:00" +%s
      time_t base_time = 1407632400;
      char* target_time_text = "2014-08-10 10:00:00";
      char* target_time_text_p1 = "2014-08-10 10:00:01";
      char* target_time_text_m1 = "2014-08-10 09:59:59";
      char* format = "%Y-%m-%d %H:%M:%S";
      
      DateCompare d(target_time_text, format);
      
      ASSERT_TRUE(d.compare(target_time_text_p1, format, DateCompare::kIS_GT));
      ASSERT_FALSE(d.compare(target_time_text, format, DateCompare::kIS_GT));
      ASSERT_FALSE(d.compare(target_time_text_m1, format, DateCompare::kIS_GT));
      
      ASSERT_TRUE(d.compare(target_time_text_p1, format, DateCompare::kIS_GTE));
      ASSERT_TRUE(d.compare(target_time_text, format, DateCompare::kIS_GTE));
      ASSERT_FALSE(d.compare(target_time_text_m1, format, DateCompare::kIS_GTE));
      
      ASSERT_TRUE(d.compare(target_time_text, format, DateCompare::kIS_EQ));
      ASSERT_FALSE(d.compare(target_time_text_p1, format, DateCompare::kIS_EQ));
      ASSERT_FALSE(d.compare(target_time_text_m1, format, DateCompare::kIS_EQ));
      
      ASSERT_TRUE(d.compare(target_time_text_m1, format, DateCompare::kIS_LT));
      ASSERT_FALSE(d.compare(target_time_text, format, DateCompare::kIS_LT));
      ASSERT_FALSE(d.compare(target_time_text_p1, format, DateCompare::kIS_LT));
      
      ASSERT_TRUE(d.compare(target_time_text_m1, format, DateCompare::kIS_LTE));
      ASSERT_TRUE(d.compare(target_time_text, format, DateCompare::kIS_LTE));
      ASSERT_FALSE(d.compare(target_time_text_p1, format, DateCompare::kIS_LTE));
    }

  } //namespace
} //namespace
