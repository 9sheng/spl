#include "spl/datespan.h"
#include <map>
#include <iostream>

#define CROL(value, width, bits) ((value << bits) | (value >> (width - bits)))
#define CROR(value, width, bits) ((value >> bits) | (value << (width - bits)))

namespace spl {

struct DateTimeCompare {
  bool operator()(const DateTime* first, const DateTime* second) {
    return *first < *second;
  }
};

DateSpan::DateSpan(const std::string& from_date,
                   const std::string& to_date,
                   const std::string& period)
    : from_date_(from_date), to_date_(to_date) {
  period_ = PeriodStrToByte(period);
}

DateSpan::DateSpan(const DateTime& from_date,
                   const DateTime& to_date,
                   char period)
    : from_date_(from_date), to_date_(to_date), period_(period) {
}

DateSpan::~DateSpan() {
}

std::string DateSpan::PeriodByteToStr(char period) {
  std::string str_period = "";
  char buf[4];

  for (int wday = 1; wday < 8; wday++) {
    if (IsWeekdaySet(period, wday)) {
      sprintf(buf, "%d", wday);
      str_period += buf;
    }
  }
  return str_period;
}

char DateSpan::PeriodStrToByte(const std::string& period) {
  char p = 0;
  for (size_t i = 0; i < period.size(); ++i) {
    p |= (1 << ((period[i]-'0') % 7));
  }
  return p;
}

DateSpan DateSpan::Intersect(const DateSpan& rhs) const {
  char new_period = period_ & rhs.period_;
  if (new_period == 0 ||
      to_date_ < rhs.from_date_ ||
      rhs.to_date_ < from_date_) {
    return DateSpan(DateTime::Null(), DateTime::Null(), 0);
  }

  DateTime new_from_date(rhs.from_date_ < rhs.from_date_ ?
                         rhs.from_date_ : from_date_);
  DateTime new_to_date(to_date_ < rhs.to_date_ ? to_date_ : rhs.to_date_);

  if (new_to_date < new_from_date) {
    return DateSpan(DateTime::Null(), DateTime::Null(), 0);
  } else {
    return DateSpan(new_from_date, new_to_date, new_period);
  }
}

void DateSpan::Substitute(const std::vector<DateSpan>& in,
                          std::vector<DateSpan>* out) const {
  out->push_back(*this);

  for (auto in_iter = in.begin(); in_iter != in.end(); ++in_iter) {
    //  +++ 代表的日期段是我们想要的
    // 两段日期没有交集
    // *in_iter  ++++++++
    // *this               --------
    if (in_iter->to_date() < this->from_date() ||
        this->to_date() < in_iter->from_date()) {
      out->push_back(*in_iter);
    } else {
      // 有交集，取左边部分
      // *in_iter ++++++-------
      // *this          -------------
      if (in_iter->from_date() < this->from_date() &&
          this->from_date() < in_iter->to_date()) {
        DateTime to_date(this->from_date());
        to_date.AddDay(-1);
        out->push_back(DateSpan(in_iter->from_date(),
                                to_date,
                                in_iter->period()));
      }
      // 有交集，取右边部分
      // *in_iter          -----+++++++
      // *this     -------------
      if (in_iter->from_date() < this->to_date() &&
          this->to_date() < in_iter->to_date()) {
        DateTime from_date(this->to_date());
        from_date.AddDay(1);
        out->push_back(DateSpan(from_date,
                                in_iter->to_date(),
                                in_iter->period()));
      }
    }
  }
}

void DateSpan::Merge(const std::vector<DateSpan>& in,
                     std::vector<DateSpan>* out) {
  typedef std::map<const DateTime*, int, DateTimeCompare> DatePoints;

  std::vector<DateSpan> tmp_out;

  static const int FROM_DATE = 0x1;
  static const int TO_DATE   = 0x2;

  DatePoints date;
  for (auto in_iter = in.begin(); in_iter != in.end(); ++in_iter) {
    date[&(in_iter->from_date())] |= FROM_DATE;
    date[&(in_iter->to_date())]   |= TO_DATE;
  }

  auto date_next_iter = date.begin();
  auto date_iter = date.begin();
  for(++date_next_iter; date_next_iter != date.end();
      ++date_iter, ++date_next_iter) {
    DateTime curr_from_date = *(date_iter->first);
    DateTime curr_to_date = *(date_next_iter->first);

    // 该日期不是终止日期，则去掉当前天，即日期前移1天
    if ((date_next_iter->second & TO_DATE) == 0) {
      curr_to_date.AddDay(1);
    }

    // 确保DatePeriod：from_date < to_date
    if (!(curr_from_date < curr_to_date)) {
      continue;
    }

    char curr_period = 0;
    for (auto in_iter = in.begin(); in_iter != in.end(); ++in_iter) {
      if (in_iter->from_date() <= curr_from_date &&
          curr_to_date <= in_iter->to_date()) {
        curr_period |= in_iter->period();
      }
    }

    if (curr_period == 0) {
      continue;
    }

    DateSpan current(curr_from_date, curr_to_date, curr_period);

    bool need_pushed = true;
    if (tmp_out.size() > 0) {
      DateSpan& last = tmp_out.back();
      if (CanBeMerged(last, current)) {
        last.set_to_date(current.to_date());
        last.set_period(last.period() | current.period());
        need_pushed = false;
      }
    }
    if (need_pushed) {
      tmp_out.push_back(current);
    }
  }

  for (auto iter = tmp_out.begin(); iter != tmp_out.end(); ++iter) {
    if (!iter->Empty()) {
      out->push_back(iter->Normalize());
    }
  }
}

bool DateSpan::CanBeMerged(const DateSpan& first, const DateSpan& second) {
  char p1 = first.period();
  char p2 = second.period();
  char same = p1 & p2;
  char p1only = p1 ^ same;
  char p2only = p2 ^ same;
  char pboth = p1 | p2;

  return !HasEffectiveDay(first.from_date(), first.to_date(), p2only) &&
    !HasEffectiveDay(second.from_date(), second.to_date(), p1only) &&
    !HasEffectiveDay(first.to_date(), second.from_date(), pboth);
}

bool DateSpan::HasEffectiveDay(const DateTime& from,
                               const DateTime& to,
                               char period) {
  if (period  == 0) {
    return false;
  }

  int day_num = to.GetDiffDayNum(from);
  if (day_num > 6) {
    return true;
  }

  int wday = from.GetWeekday();
  for (int i = 0; i < day_num; i++) {
    if (IsWeekdaySet(period, wday)) {
      return true;
    }
    IncWeekday(&wday);
  }

  return false;
}

DateSpan& DateSpan::Normalize() {
  if (period_ == 0) {
    from_date_ = DateTime::Null();
    to_date_ = DateTime::Null();
    return *this;
  }

  // 调整终止日期
  int wday = to_date_.GetWeekday();
  int day_shift = 0;
  while (true) {
    wday--;
    if (wday < 0) {
      wday += 7;
    }
    if (IsWeekdaySet(period_, wday)) {
      break;
    }
    day_shift--;
  }

  to_date_.AddDay(day_shift);

  // 调整起始日期
  wday = from_date_.GetWeekday();
  day_shift = 0;
  while (true) {
    if (IsWeekdaySet(period_, wday)) {
      break;
    }
    IncWeekday(&wday);
    day_shift++;
  }

  from_date_.AddDay(day_shift);

  int day_num = to_date_.GetDiffDayNum(from_date_);
  if (day_num < 1) {
    from_date_ = DateTime::Null();
    to_date_ = DateTime::Null();
    period_ = 0;
    return *this;
  }

  // 如果日期数小于7，则需调整period
  if (day_num < 7) {
    char effective = 0;
    for (int i = 0; i < day_num; i++) {
      effective |= (1 << wday);
      IncWeekday(&wday);
    }
    period_ &= effective;
  }

  if (period_ == 0) {
    from_date_ = DateTime::Null();
    to_date_ = DateTime::Null();
  }

  return *this;
}

DateSpan& DateSpan::ShiftDays(int day_num) {
  if (day_num != 0) {
    from_date_.AddDay(day_num);
    to_date_.AddDay(day_num);

    day_num %= 7;
    if (day_num < 0) {
      day_num += 7;
    }
    period_ = CROL(period_, 7, day_num);
  }
  return *this;
}

} // namespace spl
