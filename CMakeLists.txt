cmake_minimum_required (VERSION 2.6)

project (tigrep_project)

find_package(GTest REQUIRED)
find_package(Boost REQUIRED)

link_directories(/usr/local/lib)
link_directories(/usr/local/lib64)

add_subdirectory(src)

#tests
enable_testing()
add_subdirectory(tests)
