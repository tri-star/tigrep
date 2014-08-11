#include "application_config.h"
#include "grep_command.h"

#include <iostream>
#include <string>
#include <boost/program_options.hpp>

using namespace tigrep;
namespace po = boost::program_options;


int main(int argc, char* argv[]) {
  try {
    
    ApplicationConfig app_config;
    
    //define options
    po::options_description app_options("usage");
    
    app_options.add_options()
      ("help,h",   "show this help message.")
      ("regex,r",  po::value<std::string>(&app_config.regex_string), "specifies regex to capture date part of log.")
      ("format,F", po::value<std::string>(&app_config.format), "specifies date format in 'strftime' style format.")
      ("output,o", po::value<std::string>(&app_config.output_file), "output file path(default is to stdout).")
      ("input",    po::value<std::string>(&app_config.input_file), "input log file path.")
    ;
  
    po::positional_options_description p;
    p.add("input", -1);
    
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(app_options).positional(p).run(), vm);
    po::notify(vm);
    if(vm.count("help")) {
      std::cout << app_options << std::endl;
      return 0;
    }
    
    //GrepCommand grep_command();
    
  } catch(std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
