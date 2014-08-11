
add_executable(run_tests 
  grep_command_test.cc 
  util/date_compare_test.cc
  ../src/grep_command.h
  ../src/util/date_compare.h
)
target_link_libraries(run_tests ${TIGREP_LINK_LIBRARIES_FOR_TEST})

add_test(NAME all_test COMMAND run_tests)

