#include "permissions-server-impl.h"

int main(int argv, char* argc[]) {
    auto connection = sdbus::createSessionBusConnection();
    sdbus::ObjectPath objectPath{"/com/system/permissions"};
    sdbus::ServiceName serviceName{"com.system.permissions"};
    connection->requestName(serviceName);

    PermissionService permissionService(*connection, objectPath);

    connection->enterEventLoop();
    
}