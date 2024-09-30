#include <sdbus-c++/sdbus-c++.h>
#include "permissions-server.h"
#include <iostream>
#include <sqlite3.h>
#include <unistd.h>


class PermissionService: public sdbus::AdaptorInterfaces<com::system::permissions_adaptor> {
public:
    PermissionService(sdbus::IConnection& connection, sdbus::ObjectPath objectPath) : AdaptorInterfaces(connection, std::move(objectPath)) {
        registerAdaptor();
        this->connection_ = &connection;
        db_ = initializeDatabase("permissions.db");
    }
    ~PermissionService() {
        unregisterAdaptor();

        if(db_) {
            sqlite3_close(db_);
        }
    }
protected:
    void RequestPermission(const int32_t& permissionEnumCode) override {
        uint32_t pid;
        char buf[512];
        
        sdbus::ObjectPath destination{"/org/freedesktop/DBus"};
        sdbus::ServiceName serviceName{"org.freedesktop.DBus"};

        auto connection = sdbus::createProxy(serviceName, destination);


        connection->callMethod("GetConnectionUnixProcessID").onInterface("org.freedesktop.DBus").withArguments(getObject().getCurrentlyProcessedMessage().getSender()).storeResultsTo(pid);
        get_exe_for_pid(pid, buf, 512);

        savePermissionRequest(pid, buf, permissionEnumCode, 69);

        emitPermissionGranted(true);
        return;
    }

    bool CheckApplicationHasPermission(const std::string& applicationExecPath, const int32_t& permissionEnumCode) override {
        emitPermissionChecked(true);
        return true;
    }

private:
    sdbus::IConnection* connection_;
    sqlite3* db_;
    std::unique_ptr<sdbus::IObject> m_object_;

        sqlite3* initializeDatabase(const std::string& dbName) {
        sqlite3* db;
        if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK) {
            std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
            return nullptr;
        }

        const char* createTableSQL = "CREATE TABLE IF NOT EXISTS Permissions ("
                                     "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                                     "ClientPID INTEGER, "
                                     "ExecPath TEXT, "
                                     "PermissionCode INTEGER, "
                                     "Timestamp INTEGER);";
        
        char* errorMessage = nullptr;
        if (sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errorMessage) != SQLITE_OK) {
            std::cerr << "Error creating table: " << errorMessage << std::endl;
            sqlite3_free(errorMessage);
            sqlite3_close(db);
            return nullptr;
        }

        return db;
    }

        void savePermissionRequest(pid_t clientPID, const std::string& execPath,const int32_t& permissionCode, uint64_t timestamp) {
        const char* insertSQL = "INSERT INTO Permissions (ClientPID, ExecPath, PermissionCode, Timestamp) VALUES (?, ?, ?, ?);";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db_, insertSQL, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db_) << std::endl;
            return;
        }

        sqlite3_bind_int(stmt, 1, clientPID);
        sqlite3_bind_text(stmt, 2, execPath.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, permissionCode);
        sqlite3_bind_int64(stmt, 4, static_cast<sqlite3_int64>(timestamp));

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db_) << std::endl;
        } else {
            std::cout << "Permission request saved successfully!" << std::endl;
        }

        sqlite3_finalize(stmt);
    }

    int get_exe_for_pid(pid_t pid, char *buf, size_t bufsize) {
    char path[32];
    sprintf(path, "/proc/%d/exe", pid);
    return readlink(path, buf, bufsize);
}
};

