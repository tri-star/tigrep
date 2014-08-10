
#include <ctime>

namespace tigrep {
  namespace util {
    
    class DateCompare {
    
    public:
      
      static const int kIS_GT  = 1;
      static const int kIS_GTE = 2;
      static const int kIS_EQ  = 3;
      static const int kIS_LT  = 4;
      static const int kIS_LTE = 5;
      
      DateCompare() {
      }
      
      DateCompare(const time_t base_date_time) {
        base_date_time_ = base_date_time;
      }
      
      DateCompare(const char* base_time_string, const char* format) {
        setDateTime(base_time_string, format);
      }
      
      virtual ~DateCompare() {
      }
      
      void setDateTime(const time_t base_date_time) {
        base_date_time_ = base_date_time;
      }
      
      void setDateTime(const char* date_time_string, const char* format) {
        time_t base_date_time = getTimestampFromText(date_time_string, format);
        setDateTime(base_date_time);
      }
      
      bool compare(const time_t target_time, const int mode) {
        
        switch(mode) {
          case kIS_GT:  return target_time > base_date_time_;
          case kIS_GTE: return target_time >= base_date_time_;
          case kIS_EQ:  return target_time == base_date_time_;
          case kIS_LT:  return target_time < base_date_time_;
          case kIS_LTE: return target_time <= base_date_time_;
        }
        throw std::logic_error("invalid mode.");
      }
      
      bool compare(const char* date_time_string, const char* format, const int mode) {
        time_t target_time;
        
        target_time = getTimestampFromText(date_time_string, format);
        
        return compare(target_time, mode);
      }
      
      time_t getTimestampFromText(const char* date_time_string, const char* format) {
        struct tm tm;
        
        strptime(date_time_string, format, &tm);
        return mktime(&tm);
      }
      
    private:
      
      time_t base_date_time_;
      
    };
    
  } //namespace
}  //namespace
