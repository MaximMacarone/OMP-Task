#include "time-server-impl.h"

int main() {
    auto connection = sdbus::createSessionBusConnection();
    sdbus::ObjectPath objectPath{"/com/system/time"};
    sdbus::ServiceName serviceName{"com.system.time"};
    connection->requestName(serviceName);

    SystemTimeService service(*connection, objectPath);

    connection->enterEventLoop();
}