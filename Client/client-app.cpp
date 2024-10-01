#include <sdbus-c++/sdbus-c++.h>
#include "../Task-1/com.system.permissions/permissions-client-impl.h"
#include "../Task-2/com.system.time/time-client-impl.h"
#include <iostream>
#include <unistd.h>

void getPermission();
void getSystemTime();

int main() {
    while(true) {
        int selection;
        std::cout << "1.Request check system time permission\n";
        std::cout << "2.Get system time\n";
        std::cin >> selection;

        switch (selection)
        {
        case 1:
            getPermission();
            break;
        case 2:
            getSystemTime();
        default:
            break;
        }
    }
}

void getPermission() {
    sdbus::ServiceName destination{"com.system.permissions"};
    sdbus::ObjectPath objectPath{"/com/system/permissions"};
    
    PermissionProxy permissionProxy(std::move(destination), std::move(objectPath));
    permissionProxy.RequestPermission(0);
    sleep(1);
}

void getSystemTime() {
    sdbus::ServiceName destination{"com.system.time"};
    sdbus::ObjectPath objectPath{"/com/system/time"};

    TimeProxy timeProxy(std::move(destination), std::move(objectPath));
    try {
        timeProxy.GetSystemTime();
    } catch(const sdbus::Error& e) {
        std::cerr << e.getName() << " : " << e.getMessage() << std::endl;
    }

    sleep(1);
}