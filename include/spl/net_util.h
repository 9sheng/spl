#pragma once

#include <ifaddrs.h>

namespace spl {

int GetIpString(const std::string& name, std::string* ip) {
  struct ifaddrs *ifaddr;

  if (getifaddrs(&ifaddr) == -1) {
    return -1;
  }

  int n = 0;
  *ip = "";
  for (struct ifaddrs *ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
    if ((strncmp(ifa->ifa_name, name.c_str(), name.length()) == 0) &&
        (ifa->ifa_addr->sa_family == AF_INET)) {
      struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;
      if (!ip->empty()) {
        *ip += "/";
      }
      *ip += inet_ntoa(addr->sin_addr);
      n++;
    }
  }

  freeifaddrs(ifaddr);
  return n;
}

} // namespace spl
