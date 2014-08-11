#ifndef TIGREP_GREP_COMMAND_H_
#define TIGREP_GREP_COMMAND_H_

#include "util/date_compare.h"

#include <stdio.h>
#include <boost/regex.hpp>
#include <istream>
#include <string>
#include <ctime>

namespace tigrep {
  
  typedef struct GrepConfig {
    boost::regex pattern;
    std::string format;

    time_t start_date_time;
    time_t end_date_time;
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

    GrepCommand(std::istream& ist, std::ostream& ost, GrepConfig_t& config) {
      initialize(ist, ost, config);
    }
    
    virtual ~GrepCommand() {
    }

    void initialize(std::istream& ist, std::ostream& ost, GrepConfig_t& config) {
      ist_ = &ist;
      ost_ = &ost;
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

      while(status_ != kSTATE_END && !ist_->eof()) {

        ist_->getline(read_buffer, buffer_size);
        if(ist_->bad()) {
          throw std::runtime_error("read file error.");
        }
        current_line_++;

        //現在の行に含まれている時刻を取得する
        if(!boost::regex_search(read_buffer, matches, config_.pattern)) {
          //取得できない場合は現在の状態に応じた処理を実行
          dispatch(read_buffer);
          continue;
        }
        //取得できた場合は時刻を元に現在の状態を判定
        date_string_buffer = matches.str(1);
        updateStatus(date_string_buffer);

        //現在の状態に応じた処理を実行
        dispatch(read_buffer);
      }

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
        //Update status to kSTATE_OUTPUT if date_string exceeds config_.start_date_time.
        date_compare.setBaseDateTime(config_.start_date_time);
        if(date_compare.compare(date_string.c_str(), config_.format.c_str(), util::DateCompare::kIS_GTE)) {
          status_ = kSTATE_OUTPUT;
        }
        break;
      case kSTATE_OUTPUT:
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

  };
  
} // namespace

#endif
