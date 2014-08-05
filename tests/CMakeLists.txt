
add_executable(run_tests unit_tests.cc)
target_link_libraries(run_tests pthread libgtest.a libgtest_main.a)

add_test(NAME all_test COMMAND run_tests)

