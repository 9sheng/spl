#pragma once

namespace spl {

#ifndef FOREACH
#define FOREACH(iter, container)                                        \
  for (auto iter = (container).begin(); iter != (container).end(); ++iter)
#endif

// From Google Chrome
template <typename T, size_t N>
char(&ArraySizeHelper(T (&array)[N]))[N];
#define arraysize(array) (sizeof(spl::ArraySizeHelper(array)))

} // namespace spl
