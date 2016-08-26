#pragma once

#include <pthread.h>

/**
 * usage:
 * '''
 * class FooBar : public spl::Singleton<FooBar> {
 *  public:
 *   void DoSomething();
 *
 *  private:
 *   FooBar() {}
 *   ~FooBar() {}
 *
 *   friend class spl::Singleton<FooBar>;
 * }
 * '''
 */

namespace spl {

template<typename T>
class Singleton {
 public:
  static T* Instance()
  {
    pthread_once(&ponce_, &Singleton::Init);
    return value_;
  }

  static void Destroy()
  {
    typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
    delete value_;
  }

 protected:
  Singleton() {}
  ~Singleton() {}

 private:
  static void Init()
  {
    value_ = new T();
  }

 private:
  // disallow copy constructor and assignment operator
  Singleton(const Singleton&);
  Singleton& operator=(const Singleton&);

 private:
  static pthread_once_t ponce_;
  static T*             value_;
};

template<typename T>
pthread_once_t Singleton<T>::ponce_ = PTHREAD_ONCE_INIT;

template<typename T>
T* Singleton<T>::value_ = NULL;

} // namespace spl
