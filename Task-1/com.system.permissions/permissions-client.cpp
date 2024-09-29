#include <sdbus-c++/sdbus-c++.h>
#include "permissions-client.h"

class PermissionProxy: public sdbus::ProxyInterfaces<com::system::permissions_proxy> {
public:
    PermissionProxy(sdbus::IConnection& connection, sdbus::ServiceName destination, sdbus::ObjectPath path)
    : ProxyInterfaces(connection, std::move(destination), std::move(path))
    {
        registerProxy();
    }

    ~PermissionProxy()
    {
        unregisterProxy();
    }
};

int main() {
    auto connection = sdbus::createSessionBusConnection();
    sdbus::ServiceName destination{"com.system.permissions"};
    sdbus::ObjectPath objectPath{"/com/system/permissions"};
    
    auto permissionProxy = std::make_unique<PermissionProxy>(*connection, std::move(destination), std::move(objectPath));
    permissionProxy->RequestPermission(0);

    connection->enterEventLoop();

}