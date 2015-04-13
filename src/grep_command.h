#ifndef TIGREP_GREP_COMMAND_H_
#define TIGREP_GREP_COMMAND_H_

#include "util/date_compare.h"
#include "util/date_util.h"

#include <stdio.h>
#include <boost/regex.hpp>
#include <istream>
#include <ostream>
#include <string>
#include <ctime>

namespace tigrep {
  
  typedef struct GrepConfig {
    boost::regex pattern;
    std::string format;

    time_t start_date_time;
    time_t end_date_time;

    std::string input_file;
    std::istream* ist;
    std::ostream* ost;

  } GrepConfig_t;


  /**
   * Handle log scanning operation.
   */
  class GrepCommand {
    
  public:
    
    static const int kSTATE_SCAN   = 1; ///State while searching start position.
    static const int kSTATE_OUTPUT = 2; ///State while output log lines.
    static const int kSTATE_END    = 3; ///State when program found end line.

    static const int OUTPUT_STDOUT = 1; ///Output lines to stdout.
    static const int OUTPUT_FILE   = 2; ///Output lines to a file.

    GrepCommand() {
    }

    GrepCommand(GrepConfig_t& config) {
      initialize(config);
    }
    
    virtual ~GrepCommand() {
    }

    void initialize(GrepConfig_t& config) {
      ist_ = config.ist;
      ost_ = config.ost;
      config_ = config;
      status_ = kSTATE_SCAN;
      current_line_ = 0;
    }
    
    /**
     * Execute log scanning
     */
    void execute() {

      size_t buffer_size = 4096;
      char* read_buffer = new char[buffer_size];
      std::string date_string_buffer;
      boost::cmatch matches;

      if(config_.input_file.length() > 0 && config_.start_date_time > 0) {
        unsigned int distance = getFileSize() / 2;
        binarySearch(distance);
      }

      while(status_ != kSTATE_END) {

        ist_->getline(read_buffer, buffer_size);
        if(ist_->bad()) {
          throw std::runtime_error("read file error.");
        }
        if(ist_->eof() && read_buffer[0] == 0) {
          //If EOF found, and current line is empty, stop log scanning.
          //(Prevent double empty line sending to ost_ stream)
          break;
        }
        current_line_++;

        //Get a date part from current line.
        if(!boost::regex_search(read_buffer, matches, config_.pattern)) {
          //If current line does not include a date part,
          //continue processing according to the status_ variable.
          dispatch(read_buffer);
          continue;
        }
        //If current line includes a date part,
        //update status_ variable responding to the date part.
        date_string_buffer = matches.str(1);
        updateStatus(date_string_buffer);

        //Processing action according to the status_ variable.
        dispatch(read_buffer);
      }
      status_ = kSTATE_END;

      delete[] read_buffer;
    }


  private:
    std::istream* ist_;    ///@var Input log file stream.
    std::ostream* ost_;    ///@var output stream.
    int status_;           ///@var Operation status.
    GrepConfig_t config_;  ///@var Configure information.
    unsigned long long current_line_; ///@var Current scanning line no.


    /**
     * Dispatch actions depending on status.
     * @param line Line data.
     *
     * @todo: parameter should some "context" whitch holds more context specific values.
     */
    void dispatch(char* line) {
      switch(status_) {
      case kSTATE_SCAN:
        //Current line is out of range, read next line.
        break;
      case kSTATE_OUTPUT:
        output(line);
        break;
      case kSTATE_END:
        //End position found and current line is out of range.
        //Operation will end at next iteration.
        break;
      default:
        throw std::logic_error("Invalid operation state");
      }
    }

    /**
     * Check current context and update status if needed.
     * @var date_string Date that included current line. Currently the "context" is only this one.
     */
    void updateStatus(std::string& date_string) {
      util::DateCompare date_compare;

      switch(status_) {
      case kSTATE_SCAN:
        if(config_.start_date_time == 0) {
          status_ = kSTATE_OUTPUT;
          break;
        }
        //Update status to kSTATE_OUTPUT if date_string exceeds config_.start_date_time.
        date_compare.setBaseDateTime(config_.start_date_time);
        if(date_compare.compare(date_string.c_str(), config_.format.c_str(), util::DateCompare::kIS_GTE)) {
          status_ = kSTATE_OUTPUT;
        }
        break;
      case kSTATE_OUTPUT:
        if(config_.end_date_time == 0) {
          break;
        }
        //Update status to kSTATE_END if date_string exceeds config_.end_date_time.
        date_compare.setBaseDateTime(config_.end_date_time);
        if(date_compare.compare(date_string.c_str(), config_.format.c_str(), util::DateCompare::kIS_GT)) {
          status_ = kSTATE_END;
        }
        break;
      }
    }


    /**
     * Output line to stream.
     */
    void output(char* line) {
      *ost_ << line << "\n";
    }

    /**
     * Get file size.
     */
    unsigned int getFileSize() {
      ist_->seekg(0, ist_->end);
      unsigned int length = ist_->tellg();
      ist_->seekg(0, ist_->beg);
      return length;
    }

    /**
     * Seek position which is near from config_.start_date_time.
     * @return bool true=Position found. false=Position not found.
     */
    bool binarySearch(unsigned int position) {
      size_t buffer_size = 4096;
      char* read_buffer = new char[buffer_size];
      std::string date_string_buffer;
      boost::cmatch matches;
      unsigned int next_position;

      //If position is too small, end up searching.
      //In this case, we need full scan instead.
      if(position < 100) {
        ist_->seekg(0, ist_->beg);
        return false;
      }

      ist_->seekg(position, ist_->beg);

      time_t current_date_time;
      while(!ist_->eof()) {
        ist_->getline(read_buffer, buffer_size);
        if(ist_->bad()) {
          throw std::runtime_error("read file error.");
        }

        //Get a date part from current line.
        if(!boost::regex_search(read_buffer, matches, config_.pattern)) {
          continue;
        }

        // If date time part found, check wether current position is near from start position or not.
        date_string_buffer = matches.str(1);
        current_date_time = util::DateUtil::stringToTime(date_string_buffer.c_str(), config_.format.c_str());
        if(current_date_time < config_.start_date_time && current_date_time + (10 * 60) >= config_.start_date_time) {
          //If current position is near from start position enough, exit the scanning.
          return true;
        }

        //If current position far from start position,
        //jump current position to half above/below.
        if(current_date_time >= config_.start_date_time) {
          return binarySearch(position - position / 2);
        }
        if(current_date_time < config_.start_date_time) {
          return binarySearch(position + position / 2);
        }
      }

    }

  };
  
} // namespace

#endif
