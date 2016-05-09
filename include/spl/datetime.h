#pragma once

#include <string>
#include <time.h>

#define DAY_SECONDS (24 * 60 * 60)

namespace spl {

class DateTime {
public:
  explicit DateTime(const std::string& d, const std::string& t = "00:00:00")
      : ts_(-1) {
    char buf[32];
    snprintf(buf, sizeof(buf), "%s %s", d.c_str(), t.c_str());

    struct tm ptm = {0};
    if (strptime(buf, (char*)"%Y-%m-%d %H:%M:%S", &ptm) != NULL) {
      ts_ = mktime(&ptm);
    }
  }

  ~DateTime() { }

  bool operator<(const DateTime& rhs) const {
    return ts_ < rhs.ts_;
  }

  bool operator<=(const DateTime& rhs) const {
    return ts_ <= rhs.ts_;
  }

  inline bool IsValid() const {
    return ts_ != -1;
  }

  int GetWeekday() const {
    struct tm ptm;
    localtime_r(&ts_, &ptm);
    return ptm.tm_wday;
  }

  int GetDiffDayNum(const DateTime& rhs) const {
    return (ts_ - rhs.ts_) / DAY_SECONDS;
  }

  DateTime& AddDay(int n) {
    ts_ += n * DAY_SECONDS;
    return *this;
  }

  std::string GetDateString() const {
    if (ts_ == -1) {
      return "0000-00-00";
    }

    struct tm ptm = {0};
    localtime_r(&ts_, &ptm);

    char buf[32];
    strftime(buf, sizeof(buf), (char*)"%Y-%m-%d", &ptm);
    return buf;
  }

  static DateTime Null() {
    static DateTime null_day("0000-00-00");
    return null_day;
  }

private:
  time_t ts_;
};

} // namespace spl
