cmake_minimum_required (VERSION 2.6)

project (tigrep_project)

find_package(GTest REQUIRED)
find_package(Boost REQUIRED)

link_directories(/usr/local/lib)
link_directories(/usr/local/lib64)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin )

add_subdirectory(src)

#tests
enable_testing()
add_subdirectory(tests)
