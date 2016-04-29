#pragma once

#include <string>
#include <vector>
#include <inttypes.h>
#include <time.h>

// namespace spl {

inline void Split(std::vector<std::string>* result,
                  const std::string& s,
                  char delim) {
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
}

inline std::string ToStr(int64_t n) {
  char buf[64];
  snprintf(buf, sizeof(buf), "%" PRId64, n);
  return std::string(buf);
}

inline time_t Str2Timestamp(const std::string& str) {
  // If we don't initialize ptm, ptm.tm_isdst will be a random value.
  // `mktime` will take take much more time than expected.
  struct tm ptm = {0};
  if (strptime(str.c_str(), (char*)"%Y-%m-%d %H:%M:%S", &ptm) != NULL) {
    return mktime(&ptm);
  }
  return -1;
}

inline std::string Timestamp2Str(time_t ts) {
  struct tm ptm = {0};
  char buf[32];
  strftime(buf, sizeof(buf), (char*)"%Y-%m-%d %H:%M:%S", localtime_r(&ts, &ptm));
  return std::string(buf);
}

} // namespace spl
