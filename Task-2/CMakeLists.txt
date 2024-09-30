find_package(sdbus-c++ REQUIRED)


add_executable(time-server com.system.time/time-server.cpp)
target_link_libraries(time-server sdbus-c++)

add_executable(time-client com.system.time/time-client.cpp)
target_link_libraries(time-client sdbus-c++)
