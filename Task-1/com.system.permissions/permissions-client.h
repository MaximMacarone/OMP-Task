
/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef __sdbuscpp__permissions_client_h__proxy__H__
#define __sdbuscpp__permissions_client_h__proxy__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace com {
namespace system {

class permissions_proxy {
public:
  static constexpr const char *INTERFACE_NAME = "com.system.permissions";

protected:
  permissions_proxy(sdbus::IProxy &proxy) : m_proxy(proxy) {}

  permissions_proxy(const permissions_proxy &) = delete;
  permissions_proxy &operator=(const permissions_proxy &) = delete;
  permissions_proxy(permissions_proxy &&) = delete;
  permissions_proxy &operator=(permissions_proxy &&) = delete;

  ~permissions_proxy() = default;

  void registerProxy() {
    m_proxy.uponSignal("PermissionGranted")
        .onInterface(INTERFACE_NAME)
        .call([this](const bool &GrantedPermission) {
          this->onPermissionGranted(GrantedPermission);
        });
    m_proxy.uponSignal("PermissionChecked")
        .onInterface(INTERFACE_NAME)
        .call([this](const bool &CheckedPermission) {
          this->onPermissionChecked(CheckedPermission);
        });
  }

  virtual void onPermissionGranted(const bool &GrantedPermission) = 0;
  virtual void onPermissionChecked(const bool &CheckedPermission) = 0;

public:
  void RequestPermission(const int32_t &permissionEnumCode) {
    m_proxy.callMethod("RequestPermission")
        .onInterface(INTERFACE_NAME)
        .withArguments(permissionEnumCode);
  }

  bool CheckApplicationHasPermission(const std::string &applicationExecPath,
                                     const int32_t &permissionEnumCode) {
    bool result;
    m_proxy.callMethod("CheckApplicationHasPermission")
        .onInterface(INTERFACE_NAME)
        .withArguments(applicationExecPath, permissionEnumCode)
        .storeResultsTo(result);
    return result;
  }

private:
  sdbus::IProxy &m_proxy;
};

} // namespace system
} // namespace com

#endif
