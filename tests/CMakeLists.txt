
add_executable(run_tests 
  unit_tests.cc 
  util/date_compare_test.cc 
)
target_link_libraries(run_tests pthread libboost_regex.a libgtest.a libgtest_main.a)

add_test(NAME all_test COMMAND run_tests)

