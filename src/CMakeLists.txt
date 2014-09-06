cmake_minimum_required (VERSION 2.6)

project (tigrep_project)

find_package(GTest REQUIRED)
find_package(Boost REQUIRED)

#add library folder prefixed /usr/local
#this project assumes boost,gtest static libraries are located under /usr/local.
link_directories(/usr/local/lib)
link_directories(/usr/local/lib64)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin )


#variables for project specific.
set(TIGREP_LINK_LIBRARIES libboost_regex.a libboost_program_options.a)
set(TIGREP_LINK_LIBRARIES_FOR_TEST ${TIGREP_LINK_LIBRARIES} pthread libgtest.a libgtest_main.a )


add_executable(tigrep
  tigrep.cc
  application.h
  application_config.h
  application_config_validator.h
  log_type_repository.h
  grep_config_builder.h
  grep_command.h
  util/date_compare.h
  util/date_util.h
)
target_link_libraries(tigrep ${TIGREP_LINK_LIBRARIES})

install(TARGETS tigrep DESTINATION bin)


#tests
enable_testing()
add_subdirectory(tests)
