#include <sdbus-c++/sdbus-c++.h>
#include "permissions-server.h"
#include <iostream>


class PermissionService: public sdbus::AdaptorInterfaces<com::system::permissions_adaptor> {
public:
    PermissionService(sdbus::IConnection& connection, sdbus::ObjectPath objectPath) : AdaptorInterfaces(connection, std::move(objectPath)) {
        registerAdaptor();
        this->connection_ = &connection;
    }
    ~PermissionService() {
        unregisterAdaptor();
    }
protected:
    void RequestPermission(const int32_t& permissionEnumCode) override {
        std::string dName = connection_->getUniqueName();
        std::cout << dName;
    }

    bool CheckApplicationHasPermission(const std::string& applicationExecPath, const int32_t& permissionEnumCode) override {
        return false;
    }

private:
    sdbus::IConnection* connection_;
};

int main(int argv, char* argc[]) {
    auto connection = sdbus::createSessionBusConnection();
    sdbus::ObjectPath objectPath{"/com/system/permissions"};
    sdbus::ServiceName serviceName{"com.system.permissions"};
    connection->requestName(serviceName);

    PermissionService permissionService(*connection, objectPath);

    connection->enterEventLoopAsync();
    
}