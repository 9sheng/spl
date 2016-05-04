#pragma once

#include "spl/datetime.h"
#include <vector>

namespace spl {

class DateSpan {
public:
  DateSpan(const std::string& from_date, const std::string& to_date, const std::string& period);
  DateSpan(const DateTime& from_date, const DateTime& to_date, char period);
  ~DateSpan();

  inline const DateTime& from_date() const;
  inline void set_from_date(const DateTime& rhs);
  inline void set_from_date(const std::string& rhs);

  inline const DateTime& to_date() const;
  inline void set_to_date(const DateTime& rhs);
  inline void set_to_date(const std::string& rhs);

  inline char period() const;
  inline void set_period(char rhs);
  inline void set_period(const std::string& rhs);

  inline std::string GetPeriodStr() const;
  inline std::string ToString() const;
  inline bool Empty() const;

  DateSpan& Normalize();
  DateSpan& ShiftDays(int day_num);
  DateSpan Intersect(const DateSpan& rhs) const;
  void Substitute(const std::vector<DateSpan>& in, std::vector<DateSpan>* out) const;

  static void Merge(const std::vector<DateSpan>& in, std::vector<DateSpan>* out);

protected:
  static std::string PeriodByteToStr(char period);
  static char PeriodStrToByte(const std::string& period);
  static bool CanBeMerged(const DateSpan& first, const DateSpan& second);
  static bool HasEffectiveDay(const DateTime& start, const DateTime& end, char period);
  static inline bool IsWeekdaySet(char period, int day);
  static inline void IncWeekday(int* day);

protected:
  // 日期区间为：[from_date_, to_date_)
  // 周期位： 0:SUN, 1:MON, 2:TUS, 3:WED, 4:THU, 5:FRI, 6:SAT
  // 如果 from_date >= to_date_，则没有有效日期
  DateTime from_date_;
  DateTime to_date_;
  char period_;
};

const DateTime& DateSpan::from_date() const {
  return from_date_;
}

void DateSpan::set_from_date(const DateTime& rhs) {
  from_date_ = rhs;
}

void DateSpan::set_from_date(const std::string& rhs) {
  from_date_ = DateTime(rhs);
}

const DateTime& DateSpan::to_date() const {
  return to_date_;
}

void DateSpan::set_to_date(const DateTime& rhs) {
  to_date_ = rhs;
}

void DateSpan::set_to_date(const std::string& rhs) {
  to_date_ = DateTime(rhs);
}

char DateSpan::period() const {
  return period_;
}

void DateSpan::set_period(char rhs) {
  period_ = rhs;
}

void DateSpan::set_period(const std::string& rhs) {
  period_ = PeriodStrToByte(rhs);
}

std::string DateSpan::GetPeriodStr() const {
  return PeriodByteToStr(period_);
}

std::string DateSpan::ToString() const {
  return from_date_.GetDateString() + "|" + to_date_.GetDateString() + "|" + GetPeriodStr();
}

bool DateSpan::IsWeekdaySet(char period, int day) {
  char flag = 1 << ( day % 7);
  return (flag & period) != 0;
}

void DateSpan::IncWeekday(int* day) {
  *day = *day == 6 ? 0 : *day + 1;
}

bool DateSpan::Empty() const {
  return !HasEffectiveDay(from_date_, to_date_, period_);
}

} // namespace spl
