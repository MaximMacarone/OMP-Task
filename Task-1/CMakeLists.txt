find_package(sdbus-c++ REQUIRED)

add_executable(permissions-server com.system.permissions/permissions-server.cpp)
target_link_libraries(permissions-server sdbus-c++)

add_executable(permissions-client com.system.permissions/permissions-client.cpp)
target_link_libraries(permissions-client sdbus-c++)

