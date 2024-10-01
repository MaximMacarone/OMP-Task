
/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef __sdbuscpp__time_client_h__proxy__H__
#define __sdbuscpp__time_client_h__proxy__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace com {
namespace system {

class time_proxy {
public:
  static constexpr const char *INTERFACE_NAME = "com.system.time";

protected:
  time_proxy(sdbus::IProxy &proxy) : m_proxy(proxy) {}

  time_proxy(const time_proxy &) = delete;
  time_proxy &operator=(const time_proxy &) = delete;
  time_proxy(time_proxy &&) = delete;
  time_proxy &operator=(time_proxy &&) = delete;

  ~time_proxy() = default;

  void registerProxy() {
    m_proxy.uponSignal("AquiredSystemTime")
        .onInterface(INTERFACE_NAME)
        .call([this](const uint64_t &systemTime) {
          this->onAquiredSystemTime(systemTime);
        });
  }

  virtual void onAquiredSystemTime(const uint64_t &systemTime) = 0;

public:
  uint64_t GetSystemTime() {
    uint64_t result;
    m_proxy.callMethod("GetSystemTime")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

private:
  sdbus::IProxy &m_proxy;
};

} // namespace system
} // namespace com

#endif
