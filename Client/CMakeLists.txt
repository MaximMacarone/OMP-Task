find_package(sdbus-c++ REQUIRED)

add_executable(client-app client-app.cpp)
target_link_libraries(client-app sdbus-c++)

