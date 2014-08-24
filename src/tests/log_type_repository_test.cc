#include "../log_type_repository.h"

#include <gtest/gtest.h>

namespace tigrep {

  TEST(LogTypeRepositoryTest, basic) {
    LogTypeRepository repository;
    LogTypeEntry *entry1, *entry2;

    entry1 = new LogTypeEntry();
    entry1->name = "aaa";
    entry1->regex = "regex1";
    entry1->format = "format1";

    entry2 = new LogTypeEntry();
    entry2->name = "bbb";
    entry2->regex = "regex2";
    entry2->format = "format2";

    repository.addEntry(*entry1);
    repository.addEntry(*entry2);

    LogTypeEntry* result = repository.getEntry("aaa");

    ASSERT_TRUE(result != NULL);
    ASSERT_STREQ("regex1", result->regex.c_str());

    result = repository.getEntry("bbb");
    ASSERT_TRUE(result != NULL);
    ASSERT_STREQ("regex2", result->regex.c_str());

    result = repository.getEntry("bbc");
    ASSERT_TRUE(result == NULL);
  }

} //name space
