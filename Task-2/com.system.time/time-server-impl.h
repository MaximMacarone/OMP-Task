#include <sdbus-c++/sdbus-c++.h>
#include "time-server.h"
#include <unistd.h>

class SystemTimeService final: public sdbus::AdaptorInterfaces<com::system::time_adaptor> {
public:
    SystemTimeService(sdbus::IConnection& connection, sdbus::ObjectPath objectPath) : AdaptorInterfaces(connection, std::move(objectPath)) {
    registerAdaptor();
    }
    ~SystemTimeService() {
        unregisterAdaptor();
    }
protected:
    uint64_t GetSystemTime() override {
        uint64_t sysTime;
        bool hasPermission;

        uint32_t pid;
        char buf[512];
        
        sdbus::ObjectPath destinationDBus{"/org/freedesktop/DBus"};
        sdbus::ServiceName serviceNameDBus{"org.freedesktop.DBus"};

        auto connectionDBus = sdbus::createProxy(serviceNameDBus, destinationDBus);


        connectionDBus->callMethod("GetConnectionUnixProcessID").onInterface("org.freedesktop.DBus").withArguments(getObject().getCurrentlyProcessedMessage().getSender()).storeResultsTo(pid);
        buf[getObjectPath(pid, buf, 512)] = 0;

        sdbus::ObjectPath destination{"/com/system/permissions"};
        sdbus::ServiceName serviceName{"com.system.permissions"};
        
        auto connection = sdbus::createProxy(serviceName, destination);
        connection->callMethod("CheckApplicationHasPermission").onInterface(serviceName).withArguments(buf, 0).storeResultsTo(hasPermission);

        if(!hasPermission) {
            // error
            exit(69);
            throw;
        }
        sysTime = getCurrentTimestamp();
        emitAquiredSystemTime(sysTime);
        return sysTime;
    }
private:
    uint64_t getCurrentTimestamp(){

        auto now = std::chrono::system_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());

        return static_cast<uint64_t>(duration.count());
    }

    int getObjectPath(pid_t pid, char *buf, size_t bufsize) {
    char path[32];
    sprintf(path, "/proc/%d/exe", pid);
    return readlink(path, buf, bufsize);
    }
};