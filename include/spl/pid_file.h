#pragma once

#include <string>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

namespace spl {

class PidFile {
 public:
  PidFile(const char* pid_file_path)
      : pid_file_path_(pid_file_path),
        is_locked_(false)
  {
  }
  ~PidFile() {}

  std::string ErrorInfo()
  {
    return error_info_;
  }

  bool Lock()
  {
    int fd = open(pid_file_path_.c_str(),
                  O_WRONLY | O_CREAT,
                  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd < 0) {
      error_info_ = "Cannot open pid file";
      return false;
    }

    struct flock fl;
    fl.l_type   = F_WRLCK;
    fl.l_start  = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len    = 0;
    if (fcntl(fd, F_SETLK, &fl) < 0) {
      if (errno == EACCES || errno == EAGAIN) {
        close(fd);
        error_info_ = "Cannot lock pid file" ;
        return false;
      }
    }

    if (ftruncate(fd, 0) < 0) {
      close(fd);
      error_info_ = "Cannot truncate pid file" ;
      return false;
    }

    char buf[16];
    snprintf(buf, sizeof(buf), "%ld", (long)getpid());
    if (write(fd, buf, strlen(buf)) != (ssize_t)strlen(buf)) {
      close(fd);
      error_info_ = "Cannot write pid file" ;
      return false;
    }

    int val = fcntl(fd, F_GETFD, 0);
    if (val < 0) {
      close(fd);
      error_info_ = "Cannot get pid file descriptor" ;
      return false;
    }

    val |= FD_CLOEXEC;
    if (fcntl(fd, F_SETFD, val) < 0) {
      close(fd);
      error_info_ = "Cannot set pid file attribute" ;
      return false;
    }

    is_locked_ = true;
    return true;
  }

  void Release()
  {
    if (!is_locked_) {
      return;
    }

    if (unlink(pid_file_path_.c_str()) == 0) {
      is_locked_ = false;
    } else {
      error_info_ = "Release pid file failed" ;
    }
  }

 private:
  PidFile(const PidFile& rhs);
  PidFile& operator=(const PidFile& rhs) const;

 private:
  std::string pid_file_path_;
  bool is_locked_;
  std::string error_info_;
};

} // namespace spl
