find_package(sdbus-c++ REQUIRED)

find_package (SQLite3)

include_directories(${SQLite3_INCLUDE_DIRS})

add_executable(permissions-server com.system.permissions/permissions-server.cpp)
target_link_libraries(permissions-server sdbus-c++)
target_link_libraries(permissions-server sqlite3)


