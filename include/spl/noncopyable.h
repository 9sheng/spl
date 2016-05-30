#pragma once

/**
 * Usage:
 * Class FooBar : private spl::Noncopyable {
 *  public:
 *   FooBar() {}
 * };
 */

namespace spl {

class Noncopyable {
  Noncopyable() {};
  Noncopyable(const Noncopyable&) = delete;
  Noncopyable& operator=(const Noncopyable&) = delete;
};

} // namespace spl
