#include <sdbus-c++/sdbus-c++.h>
#include "time-client.h"
#include <iostream>

class TimeProxy final: public sdbus::ProxyInterfaces<com::system::time_proxy> {
public:
    TimeProxy(sdbus::ServiceName destination, sdbus::ObjectPath path)
    : ProxyInterfaces(std::move(destination), std::move(path))
    {
        registerProxy();
    }

    ~TimeProxy()
    {
        unregisterProxy();
    }
protected:
    void onAquiredSystemTime(const uint64_t& systemTime) {
        std::cout << systemTime << std::endl;
    }
};