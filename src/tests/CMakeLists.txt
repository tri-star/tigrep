
add_executable(run_tests
  application_config_validator_test.cc
  grep_config_builder_test.cc
  grep_command_test.cc
  log_type_repository_test.cc
  util/date_compare_test.cc
  util/date_util_test.cc
  ../application_config_validator.h
  ../grep_config_builder.h
  ../grep_command.h
  ../log_type_repository.h
  ../util/date_compare.h
  ../util/date_util.h
  
)
target_link_libraries(run_tests ${TIGREP_LINK_LIBRARIES_FOR_TEST})

add_test(NAME all_test COMMAND run_tests)

