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

void getValidateErrorMessage(ApplicationConfigValidator& validator, std::string *message_buffer);
void prepareLogTypes(tigrep::LogTypeRepository* repository);
void listLogTypes(tigrep::LogTypeRepository& repository);

int main(int argc, char* argv[]) {
  try {
    
    //Initialize log types.
    tigrep::LogTypeRepository log_type_repository;
    prepareLogTypes(&log_type_repository);

    ApplicationConfig app_config;
    
    po::options_description app_options("usage");
    app_options.add_options()
      ("help,h",   "show this help message.")
      ("list-log-types,L", "list all log types.")
      ("log-type",  po::value<std::string>(&app_config.log_type),     "specifies log type. see --list-log-types option for valid log types.")
      ("regex,r",   po::value<std::string>(&app_config.regex_string), "specifies regex to capture date part of log. This parameter is required if log-type is omitted.")
      ("format,F",  po::value<std::string>(&app_config.format),       "specifies date format in 'strftime' style format. This parameter is required if log-type is omitted.")
      ("output,o",  po::value<std::string>(&app_config.output_file),  "output file path(default is to stdout).")
      ("input",     po::value<std::string>(&app_config.input_file),   "input log file path.")
      ("from,f",    po::value<std::string>(&app_config.start_time),   "start date time to extract. The format is same as 'format' option.")
      ("to,t",      po::value<std::string>(&app_config.end_time),     "end date time to extract. The format is same as 'format' option.")
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
    if(vm.count("list-log-types")) {
      listLogTypes(log_type_repository);
      return 0;
    }
    
    ApplicationConfigValidator validator;
    if(!validator.validate(app_config)) {
      std::string error_string;
      getValidateErrorMessage(validator, &error_string);
      std::cerr << error_string << std::endl;
      return 1;
    }
    
    GrepConfig_t grep_config;
    grep_config.pattern = boost::regex(app_config.regex_string);
    grep_config.format = app_config.format;
    grep_config.start_date_time = app_config.getStartTime();
    grep_config.end_date_time = app_config.getEndTime();

    //Setup regex and format option according to log type if it specified.
    tigrep::LogTypeEntry* log_type_entry;

    if(app_config.log_type.length() > 0) {
      log_type_entry = log_type_repository.getEntry(app_config.log_type);
      if(log_type_entry == NULL) {
        std::cerr << "Invalid log type." << std::endl;
        return 1;
      }

      grep_config.pattern = boost::regex(log_type_entry->regex);
      grep_config.format = log_type_entry->format;
    }

    //Setup input/output stream responding to input parameters.
    //If no input file names given, use stdin/stdout instead.
    std::istream* ist_for_command = &std::cin;
    std::ostream* ost_for_command = &std::cout;
    std::ifstream ifst;
    std::ofstream ofst;
    if(app_config.isInputFileSpecified()) {
      ifst.open(app_config.input_file.c_str());
      if(ifst.fail()) {
        throw std::runtime_error("Failed to open input file.");
      }
      ist_for_command = &ifst;
    }
    if(app_config.isOutputFileSpecified()) {
      ofst.open(app_config.output_file.c_str());
      if(ofst.fail()) {
        throw std::runtime_error("Failed to open output file in write mode.");
      }
      ost_for_command = &ofst;
    }
    
    ist_for_command->exceptions(std::ios::badbit);
    ost_for_command->exceptions(std::ios::badbit);
    GrepCommand grep_command(ist_for_command, ost_for_command, grep_config);
    grep_command.execute();
    
  } catch(std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}

void getValidateErrorMessage(ApplicationConfigValidator& validator, std::string *message_buffer) {
  std::list<std::string> errors;
  std::list<std::string>::iterator error_iterator;
  validator.getErrors(&errors);
  
  for(error_iterator = errors.begin(); error_iterator != errors.end(); error_iterator++) {
    message_buffer->append(*error_iterator);
    message_buffer->append("\n");
  }
}


void prepareLogTypes(tigrep::LogTypeRepository* repository) {
  tigrep::LogTypeEntry* entry[3];

  entry[0] = new LogTypeEntry();
  entry[0]->name = "mysql-general";
  entry[0]->regex = "^([0-9]{6} [0-9:]{8})";
  entry[0]->format = "%y%m%d %H:%M:%S";

  entry[1] = new LogTypeEntry();
  entry[1]->name = "mysql-slow";
  entry[1]->regex = "^# Time: ([0-9\\s:]+)";
  entry[1]->format = "%y%m%d %H:%M:%S";

  //e.g. Aug 17 15:28:22
  entry[2] = new LogTypeEntry();
  entry[2]->name = "syslog";
  entry[2]->regex = "^(.+? [0-9]+ [0-9:]+) ";
  entry[2]->format = "%b %d %H:%M:%S";


  repository->addEntry(*entry[0]);
  repository->addEntry(*entry[1]);
  repository->addEntry(*entry[2]);
}


void listLogTypes(tigrep::LogTypeRepository& repository) {
  tigrep::LogTypeMap log_type_map = repository.getList();
  tigrep::LogTypeMap::iterator i;
  tigrep::LogTypeEntry* entry;

  std::cout << "Available log types:" << std::endl;

  for(i = log_type_map.begin(); i != log_type_map.end(); i++) {
    entry = i->second;
    std::cout << "* " << entry->name << std::endl;
    std::cout << "  - Regex:  " << entry->regex << std::endl;
    std::cout << "  - Format: " << entry->format << std::endl;
  }
}
