#ifndef TIGREP_GREP_CONFIG_BUILDER_H_
#define TIGREP_GREP_CONFIG_BUILDER_H_

#include "grep_command.h"
#include "application_config.h"
#include "log_type_repository.h"
#include "util/date_util.h"

#include <boost/regex.hpp>
#include <fstream>
#include <string>

namespace tigrep {

  class GrepConfigBuilder {

  public:

    GrepConfigBuilder() {
    }

    virtual ~GrepConfigBuilder() {

    }

    void build(ApplicationConfig& app_config, LogTypeRepository& log_type_repository, GrepConfig* grep_config) {
      std::string* regex_string = &app_config.regex_string;
      std::string* format = &app_config.format;

      //Setup regex and format option according to log type if it specified.
      LogTypeEntry* log_type_entry;
      if(app_config.log_type.length() > 0) {
        log_type_entry = log_type_repository.getEntry(app_config.log_type);
        if(log_type_entry == NULL) {
          throw std::logic_error("Invalid log type.");
          return;
        }

        regex_string = &log_type_entry->regex;
        format = &log_type_entry->format;
      }

      grep_config->pattern = boost::regex(*regex_string);
      grep_config->format = *format;
      grep_config->start_date_time = util::DateUtil::stringToTime(app_config.start_time, *format);
      grep_config->end_date_time = util::DateUtil::stringToTime(app_config.end_time, *format);

      if(app_config.isInputFileSpecified()) {
        grep_config->input_file = app_config.input_file;
      }

      setupIOStream(app_config, grep_config->ist, grep_config->ost);
    }

    /**
     * Setup input/output stream according to app_config.
     * If no input/output files are supplied, use stdin/stdout instead.
     */
    void setupIOStream(ApplicationConfig app_config, std::istream*& ist, std::ostream*& ost) {
      ist = &std::cin;
      ost = &std::cout;

      //Setup input/output stream responding to input parameters.
      //If no input file names given, use stdin/stdout instead.
      if(app_config.isInputFileSpecified()) {

        ifst_.open(app_config.input_file.c_str());
        if(ifst_.fail()) {
          throw std::runtime_error("Failed to open input file.");
        }
        ist = &ifst_;
      }
      if(app_config.isOutputFileSpecified()) {
        ofst_.open(app_config.output_file.c_str());
        if(ofst_.fail()) {
          throw std::runtime_error("Failed to open output file in write mode.");
        }
        ost = &ofst_;
      }
      ist->exceptions(std::ios::badbit);
      ost->exceptions(std::ios::badbit);
    }


  private:

    std::ifstream ifst_;
    std::ofstream ofst_;
  };

} //namespace

#endif
