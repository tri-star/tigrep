#ifndef __TIGREP_TEST_LIB_UTIL_H__
#define __TIGREP_TEST_LIB_UTIL_H__

#include <string.h>
#include <unistd.h>
#include <libgen.h>

namespace tigrep {

  class TestUtil {

  public:

    static void getFixtureDirPath(std::string* fixture_dir) {
      std::string bin_dir;

      getBinDir(&bin_dir);

      *fixture_dir = bin_dir;
      fixture_dir->append("/../src/tests/fixtures");
    }

    static void getBinDir(std::string* bin_dir_string) {
      char proc_exe_path[256];
      char bin_path[256];
      memset(proc_exe_path, 0, 256);
      memset(bin_path, 0, 256);

      sprintf(proc_exe_path, "/proc/%d/exe", getpid());
      readlink(proc_exe_path, bin_path, 255);

      dirname(bin_path);
      *bin_dir_string = bin_path;
    }

  };

}// namespace

#endif
