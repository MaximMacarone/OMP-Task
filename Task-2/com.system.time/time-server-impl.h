#include "../../utils/getObjectPath.h"
#include "time-server.h"
#include <iostream>
#include <sdbus-c++/sdbus-c++.h>
#include <unistd.h>

class SystemTimeService final
    : public sdbus::AdaptorInterfaces<com::system::time_adaptor> {
public:
  SystemTimeService(sdbus::IConnection &connection,
                    sdbus::ObjectPath objectPath)
      : AdaptorInterfaces(connection, std::move(objectPath)) {
    registerAdaptor();
  }
  ~SystemTimeService() { unregisterAdaptor(); }

protected:
  uint64_t GetSystemTime() override {
    uint64_t sysTime;
    bool hasPermission;
    uint32_t pid;

    sdbus::ObjectPath destinationDBus{"/org/freedesktop/DBus"};
    sdbus::ServiceName serviceNameDBus{"org.freedesktop.DBus"};

    auto connectionDBus = sdbus::createProxy(serviceNameDBus, destinationDBus);
    try {
      connectionDBus->callMethod("GetConnectionUnixProcessID")
          .onInterface("org.freedesktop.DBus")
          .withArguments(getObject().getCurrentlyProcessedMessage().getSender())
          .storeResultsTo(pid);
    } catch (sdbus::Error &e) {
      std::cerr << "Error: " << e.what();
    }

    std::string execPath = getObjectPath(pid);

    sdbus::ObjectPath destination{"/com/system/permissions"};
    sdbus::ServiceName serviceName{"com.system.permissions"};

    auto connection = sdbus::createProxy(serviceName, destination);
    try {
      connection->callMethod("CheckApplicationHasPermission")
          .onInterface(serviceName)
          .withArguments(execPath, 0)
          .storeResultsTo(hasPermission);
    } catch (sdbus::Error &e) {
      std::cerr << "Error: " << e.what();
    }

    if (!hasPermission) {
      throw sdbus::Error(sdbus::Error::Name{"com.system.time.Error"},
                         "Unauthorized access");
    }
    sysTime = getCurrentTimestamp();
    emitAquiredSystemTime(sysTime);
    return sysTime;
  }

private:
  uint64_t getCurrentTimestamp() {

    auto now = std::chrono::system_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::seconds>(
        now.time_since_epoch());

    return static_cast<uint64_t>(duration.count());
  }
};