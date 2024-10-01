find_package(sdbus-c++ REQUIRED)


add_executable(time-server com.system.time/time-server.cpp)
target_link_libraries(time-server sdbus-c++)
