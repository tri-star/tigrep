#include "application.h"
#include "application_config.h"
#include "application_config_validator.h"
#include "log_type_repository.h"
#include "grep_command.h"

#include <iostream>
#include <fstream>
#include <string>
#include <boost/program_options.hpp>

using namespace tigrep;
namespace po = boost::program_options;


int main(int argc, char* argv[]) {
  try {
    
    Application application;
    application.run(argc, argv);
    
  } catch(std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}


