#ifndef TIGREP_APPLICATION_H_
#define TIGREP_APPLICATION_H_

#include "application_config.h"
#include "application_config_validator.h"
#include "log_type_repository.h"
#include "grep_command.h"

#include <boost/regex.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include <string>

namespace po = boost::program_options;

namespace tigrep {
  
  /**
   * A class to separate CLI application and application logic layer.
   * This class implements such like input parameter validation and
   * input/output stream setup for application main logic.
   */
  class Application {
    
  public:
    
    Application() {
    }
    
    virtual ~Application() {
    }
    
    void run(int argc, char* argv[]) {

      //Initialize log types.
      prepareLogTypes(&log_type_repository_);

      initializeProgramOptions(argc, argv);

      std::string errors;
      if(vm_.count("help")) {
        std::cout << app_options_ << std::endl;
        return;
      }
      if(vm_.count("list-log-types")) {
        listLogTypes(log_type_repository_);
        return;
      }

      if(!validateInput(app_config_, &errors)) {
        throw std::logic_error(errors.c_str());
        return;
      }

      //Setup configration from input parameters.
      GrepConfig_t grep_config;

      //Setup regex and format option according to log type if it specified.
      LogTypeEntry* log_type_entry;
      if(app_config_.log_type.length() > 0) {
        log_type_entry = log_type_repository_.getEntry(app_config_.log_type);
        if(log_type_entry == NULL) {
          throw std::logic_error("Invalid log type.");
          return;
        }

        app_config_.regex_string = log_type_entry->regex;
        app_config_.format = log_type_entry->format;
      }

      grep_config.pattern = boost::regex(app_config_.regex_string);
      grep_config.format = app_config_.format;
      grep_config.start_date_time = app_config_.getStartTime();
      grep_config.end_date_time = app_config_.getEndTime();

      //Setup input/output stream responding to input parameters.
      //If no input file names given, use stdin/stdout instead.
      std::istream* ist_for_command = &std::cin;
      std::ostream* ost_for_command = &std::cout;
      std::ifstream ifst;
      std::ofstream ofst;
      if(app_config_.isInputFileSpecified()) {
        ifst.open(app_config_.input_file.c_str());
        if(ifst.fail()) {
          throw std::runtime_error("Failed to open input file.");
        }
        ist_for_command = &ifst;
      }
      if(app_config_.isOutputFileSpecified()) {
        ofst.open(app_config_.output_file.c_str());
        if(ofst.fail()) {
          throw std::runtime_error("Failed to open output file in write mode.");
        }
        ost_for_command = &ofst;
      }
      ist_for_command->exceptions(std::ios::badbit);
      ost_for_command->exceptions(std::ios::badbit);

      //Run grep command.
      GrepCommand grep_command(ist_for_command, ost_for_command, grep_config);
      grep_command.execute();
    }
    
  private:
    
    ApplicationConfig app_config_;
    GrepCommand grepCommand_;
    LogTypeRepository log_type_repository_;

    //Program options.
    po::options_description app_options_;
    po::positional_options_description positional_options_;
    po::variables_map vm_;
    
    void initializeProgramOptions(int argc, char* argv[]) {

      app_options_.add_options()
        ("help,h",   "show this help message.")
        ("list-log-types,L", "list all log types.")
        ("log-type",  po::value<std::string>(&app_config_.log_type),     "specifies log type. see --list-log-types option for valid log types.")
        ("regex,r",   po::value<std::string>(&app_config_.regex_string), "specifies regex to capture date part of log. This parameter is required if log-type is omitted.")
        ("format,F",  po::value<std::string>(&app_config_.format),       "specifies date format in 'strftime' style format. This parameter is required if log-type is omitted.")
        ("output,o",  po::value<std::string>(&app_config_.output_file),  "output file path(default is to stdout).")
        ("input",     po::value<std::string>(&app_config_.input_file),   "input log file path.")
        ("from,f",    po::value<std::string>(&app_config_.start_time),   "start date time to extract. The format is same as 'format' option.")
        ("to,t",      po::value<std::string>(&app_config_.end_time),     "end date time to extract. The format is same as 'format' option.")
      ;
    
      positional_options_.add("input", -1);

      po::store(po::command_line_parser(argc, argv).options(app_options_).positional(positional_options_).run(), vm_);
      po::notify(vm_);
    }


    void prepareLogTypes(LogTypeRepository* repository) {
      LogTypeEntry* entry[3];

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


    void listLogTypes(LogTypeRepository& repository) {
      LogTypeMap log_type_map = repository.getList();
      LogTypeMap::iterator i;
      LogTypeEntry* entry;

      std::cout << "Available log types:" << std::endl;

      for(i = log_type_map.begin(); i != log_type_map.end(); i++) {
        entry = i->second;
        std::cout << "* " << entry->name << std::endl;
        std::cout << "  - Regex:  " << entry->regex << std::endl;
        std::cout << "  - Format: " << entry->format << std::endl;
      }
    }


    /**
     * @return boolean true = Input data is valid. false = Input data is invalid(Error).
     */
    bool validateInput(const ApplicationConfig& app_config, std::string *error_messages) {
      ApplicationConfigValidator validator;
      if(!validator.validate(app_config)) {
        std::list<std::string> errors;
        std::list<std::string>::iterator error_iterator;
        validator.getErrors(&errors);

        for(error_iterator = errors.begin(); error_iterator != errors.end(); error_iterator++) {
          error_messages->append(*error_iterator);
          error_messages->append("\n");
        }
        return false;
      }
      return true;
    }

  };


} //name space

#endif
