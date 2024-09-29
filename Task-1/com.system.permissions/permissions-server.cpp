#include <sdbus-c++/sdbus-c++.h>
#include "permissions-server.h"

class PermissionService: public sdbus::AdaptorInterfaces<com::system::permissions_adaptor> {
public:
    PermissionService(sdbus::IConnection& connection, sdbus::ObjectPath objectPath) : AdaptorInterfaces(connection, std::move(objectPath)) {
        registerAdaptor();
    }
    ~PermissionService() {
        unregisterAdaptor();
    }
protected:
    void RequestPermission(const int32_t& permissionEnumCode) override {
        
    }

    bool CheckApplicationHasPermission(const std::string& applicationExecPath, const int32_t& permissionEnumCode) override {
        return false;
    }
};

int main(int argv, char* argc[]) {
    auto connection = sdbus::createSessionBusConnection();
    sdbus::ObjectPath objectPath{"/com/system/permissions"};
    sdbus::ServiceName serviceName{"com.system.permissions"};
    connection->requestName(serviceName);

    PermissionService permissionService(*connection, objectPath);

    connection->enterEventLoop();
}