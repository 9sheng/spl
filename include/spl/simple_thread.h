#pragma once

#include <pthread.h>

namespace spl {

class SimpleThread {
 public:
  SimpleThread() {}
  virtual ~SimpleThread() {}

  bool Start()
  {
    int ret = pthread_create(&thread_, NULL, RunMethod, static_cast<void*>(this));
    if (ret != 0) {
      thread_ = 0;
      return false;
    }

    return true;
  }

  bool Join()
  {
    void* result = NULL;
    int ret = pthread_join(thread_, &result);
    if (ret != 0) {
      thread_ = 0;
      return false;
    }

    return true;
  }

 protected:
  virtual void* Run() = 0;

 private:
  SimpleThread(const SimpleThread&);
  SimpleThread& operator=(const SimpleThread&);

  static void* RunMethod(void* ptr)
  {
    SimpleThread* thisObject = reinterpret_cast<SimpleThread*>(ptr);
    return thisObject->Run();
  }

 private:
  pthread_t thread_;
};

} // namespace spl
