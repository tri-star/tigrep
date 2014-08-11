
add_executable(tigrep
  tigrep.cc 
  grep_command.h
  util/date_compare.h
)
target_link_libraries(tigrep ${TIGREP_LINK_LIBRARIES})
