#include "time-client-impl.h"

int main() {
    sdbus::ServiceName destination{"com.system.time"};
    sdbus::ObjectPath objectPath{"/com/system/time"};
    
    TimeProxy timeProxy(std::move(destination), std::move(objectPath));
    timeProxy.GetSystemTime();
}