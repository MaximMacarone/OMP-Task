#include "time-client.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sdbus-c++/sdbus-c++.h>

class TimeProxy final : public sdbus::ProxyInterfaces<com::system::time_proxy> {
public:
  TimeProxy(sdbus::ServiceName destination, sdbus::ObjectPath path)
      : ProxyInterfaces(std::move(destination), std::move(path)) {
    registerProxy();
  }

  ~TimeProxy() { unregisterProxy(); }

protected:
  void onAquiredSystemTime(const uint64_t &systemTime) {
    std::time_t currentTime = static_cast<std::time_t>(systemTime);
    std::tm* localTime = std::localtime(&currentTime);

    std::ostringstream formattedTime;
    formattedTime << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
    std::cout << formattedTime.str() << std::endl;
  }
};