#pragma once

#include <string>
#include <vector>
#include <inttypes.h>
#include <time.h>
#include <string.h>

namespace spl {

inline std::string& TrimRight(std::string& str,
                              const std::string& chars = " \t\r\n")
{
  size_t endpos = str.find_last_not_of(chars);
  if (std::string::npos != endpos) {
    str = str.substr(0, endpos+1);
  }
  return str;
}

inline std::string& TrimLeft(std::string& str,
                             const std::string& chars = " \t\r\n")
{
  size_t startpos = str.find_first_not_of(chars);
  if (string::npos != startpos) {
    str = str.substr(startpos);
  }
  return str;
}

inline std::string& Trim(std::string& str,
                         const std::string& chars = " \t\r\n")
{
  return TrimLeft(TrimRight(str, chars), chars);
}

inline void Split(std::vector<std::string>* result,
                  const std::string& s,
                  char delim,
                  bool trim_space = false)
{
  result->clear();

  size_t last = 0;
  for (;;) {
    size_t index = s.find_first_of(delim, last);
    result->push_back(s.substr(last, index - last));
    if (index == std::string::npos) {
      break;
    }
    last = index + 1;
  }

  if (trim_space) {
    for (auto iter = result->begin(); iter != result->end(); ++iter) {
      Trim(*iter);
    }
  }
}

inline std::string ToString(int64_t n)
{
  char buf[64];
  snprintf(buf, sizeof(buf), "%" PRId64, n);
  return std::string(buf);
}

inline time_t String2Timestamp(const std::string& str,
                               const char* format = "%F %T")
{
  // If we don't initialize ptm, ptm.tm_isdst will be a random value.
  // `mktime` will take take much more time than expected.
  struct tm ptm;
  bzero(&ptm, sizeof(ptm));
  if (strptime(str.c_str(), format, &ptm) != NULL) {
    return mktime(&ptm);
  }
  return -1;
}

inline std::string Timestamp2String(time_t ts,
                                    const char* format = "%F %T")
{
  struct tm ptm;
  bzero(&ptm, sizeof(ptm));
  char buf[64];
  strftime(buf, sizeof(buf), format, localtime_r(&ts, &ptm));
  return std::string(buf);
}

} // namespace spl
