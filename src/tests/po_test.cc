#include <iostream>
#include <boost/program_options.hpp>
#include <gtest/gtest.h>

namespace po = boost::program_options;

TEST(program_options, basic) {
  
  po::options_description t("description");
  
  int opt;
  
  t.add_options()
    ("help", "help message.")
    ("opt", po::value<int>(&opt), "optimazation")
  ;
  
  po::variables_map vm;
  char* av[2];
  av[0] = "aaa";
  av[1] = "--help";  
  po::store(po::parse_command_line(2, av, t), vm);
  po::notify(vm);
  
  if(vm.count("help")) {
    std::cout << t << std::endl;
  }
  
  
}