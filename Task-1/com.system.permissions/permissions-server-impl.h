#include <sdbus-c++/sdbus-c++.h>
#include "permissions-server.h"
#include <iostream>
#include <sqlite3.h>
#include <unistd.h>
#include <chrono>
#include "../../utils/getObjectPath.h"


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
        
        sdbus::ObjectPath destination{"/org/freedesktop/DBus"};
        sdbus::ServiceName serviceName{"org.freedesktop.DBus"};

        auto connection = sdbus::createProxy(serviceName, destination);

        connection->callMethod("GetConnectionUnixProcessID").onInterface("org.freedesktop.DBus").withArguments(getObject().getCurrentlyProcessedMessage().getSender()).storeResultsTo(pid);
        std::string execPath = getObjectPath(pid);
        savePermissionRequest(pid, execPath, permissionEnumCode);

        emitPermissionGranted(true);
        return;
    }

    bool CheckApplicationHasPermission(const std::string& applicationExecPath, const int32_t& permissionEnumCode) override {
        const char* selectSQL = "SELECT COUNT(*) FROM Permissions WHERE ExecPath = ? AND PermissionCode = ?;";
    
        sqlite3_stmt* stmt;
        int result = 0;
        
        if (sqlite3_prepare_v2(db_, selectSQL, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, applicationExecPath.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_int(stmt, 2, permissionEnumCode);
            
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                result = sqlite3_column_int(stmt, 0);
            }

            sqlite3_finalize(stmt);
        } else {
            std::cerr << "Failed to prepare SQL statement: " << sqlite3_errmsg(db_) << std::endl;
        }

        bool hasPermission = result > 0;
        emitPermissionChecked(hasPermission);
        return hasPermission;
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
                                     "PermissionCode INTEGER);";
        
        char* errorMessage = nullptr;
        if (sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errorMessage) != SQLITE_OK) {
            std::cerr << "Error creating table: " << errorMessage << std::endl;
            sqlite3_free(errorMessage);
            sqlite3_close(db);
            return nullptr;
        }

        return db;
    }

        void savePermissionRequest(pid_t clientPID, const std::string& execPath,const int32_t& permissionCode) {
        const char* insertSQL = "INSERT INTO Permissions (ClientPID, ExecPath, PermissionCode) VALUES (?, ?, ?);";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db_, insertSQL, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db_) << std::endl;
            return;
        }

        sqlite3_bind_int(stmt, 1, clientPID);
        sqlite3_bind_text(stmt, 2, execPath.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, permissionCode);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db_) << std::endl;
        } else {
            std::cout << "Permission request saved successfully!" << std::endl;
        }

        sqlite3_finalize(stmt);
    }
};

