cmake_minimum_required(VERSION 3.22)
project(Task-2)

find_package(sdbus-c++ REQUIRED)


add_executable(time-server com.system.time/time-server.cpp)
target_link_libraries(time-server sdbus-c++)
