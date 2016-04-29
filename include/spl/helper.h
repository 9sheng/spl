#pragma once

namespace spl {

#ifndef FOREACH
#define FOREACH(iter, container)                                        \
  for (auto iter = (container).begin(); iter != (container).end(); ++iter)
#endif

#define DISALLOW_COPY_AND_ASSIGN(TypeName)      \
  TypeName(const TypeName&);                    \
  void operator=(const TypeName&)

// From Google Chrome
template <typename T, size_t N>
char(&ArraySizeHelper(T (&array)[N]))[N];
#define arraysize(array) (sizeof(spl::ArraySizeHelper(array)))

} // namespace spl
