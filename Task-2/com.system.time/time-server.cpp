#include <sdbus-c++/sdbus-c++.h>
#include <chrono>
#include "time-server.h"

class SystemTime: public sdbus::AdaptorInterfaces<com::system::time_adaptor> {
public:
    SystemTime(sdbus::IConnection& connection, sdbus::ObjectPath objectPath) : AdaptorInterfaces(connection, std::move(objectPath)) {
        registerAdaptor();
    }
    ~SystemTime() {
        unregisterAdaptor();
    }
protected:
    uint64_t GetSystemTime() override {
        auto now = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());
        return static_cast<uint64_t>(duration.count());
    }
};

int main() {
    
}