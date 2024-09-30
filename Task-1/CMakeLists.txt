find_package(sdbus-c++ REQUIRED)

find_package (SQLite3)

include_directories(${SQLite3_INCLUDE_DIRS})

add_executable(server com.system.permissions/server.cpp)
target_link_libraries(server sdbus-c++)
target_link_libraries(server sqlite3)

add_executable(client com.system.permissions/client.cpp)
target_link_libraries(client sdbus-c++)

