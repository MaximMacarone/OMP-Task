#include "permissions-client-impl.h"

int main() {
    sdbus::ServiceName destination{"com.system.permissions"};
    sdbus::ObjectPath objectPath{"/com/system/permissions"};
    
    PermissionProxy permissionProxy(std::move(destination), std::move(objectPath));
    permissionProxy.RequestPermission(0);
}