#include "permissions-client.h"
#include <iostream>
#include <sdbus-c++/sdbus-c++.h>

class PermissionProxy
    : public sdbus::ProxyInterfaces<com::system::permissions_proxy> {
public:
  PermissionProxy(sdbus::ServiceName destination, sdbus::ObjectPath path)
      : ProxyInterfaces(std::move(destination), std::move(path)) {
    registerProxy();
  }

  ~PermissionProxy() { unregisterProxy(); }

protected:
  void onPermissionGranted(const bool &PermissionGranted) {
    std::cout << "Permission granted\n";
  }
  void onPermissionChecked(const bool &CheckedPermission) {
    std::cout << "Permission checked\n";
  }
};
