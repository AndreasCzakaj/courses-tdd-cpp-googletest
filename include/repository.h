#pragma once

#include <string>
#include <vector>
#include <functional>
#include <optional>
#include <algorithm>
#include <sqlite3.h>
#include <stdexcept>
#include <memory>

// ============================================================================
// Repository Pattern - Generic DAO Interface
// ============================================================================

template<typename T>
class IRepository {
public:
    virtual ~IRepository() = default;
    virtual std::optional<T> get(const std::string& id) = 0;
};


class RepositoryException : public std::exception {
private:
    std::string message;

public:
    explicit RepositoryException(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

// ============================================================================
// Vector-based Repository Implementation
// ============================================================================

template<typename T>
class VectorRepository : public IRepository<T> {
private:
    std::vector<T> data;
    std::function<bool(const T&, const std::string&)> filterFn;

public:
    // Constructor with filter function and optional initial data
    VectorRepository(
        std::function<bool(const T&, const std::string&)> filter,
        const std::vector<T>& initialData = {}
    ) : filterFn(filter), data(initialData) {}

    std::optional<T> get(const std::string& id) override {
        auto it = std::find_if(data.begin(), data.end(), [this, &id](const T& item) {
            return filterFn(item, id);
        });

        if (it != data.end()) {
            return *it;
        }
        return std::nullopt;
    }

    // Helper method to add data (useful for testing)
    void add(const T& item) {
        data.push_back(item);
    }
};

template<typename T>
class ThrowingRepository : public IRepository<T> {
public:
    std::optional<T> get(const std::string& id) override {
        throw RepositoryException("oops");        
    }
};

// ============================================================================
// SQLite-based Repository Implementation
// ============================================================================

template<typename T>
class SqliteRepository : public IRepository<T> {
private:
    sqlite3* db;
    std::string tableName;
    std::function<T(sqlite3_stmt*)> rowMapper;
    std::string colName;
    bool ownsDb;

    void checkSqliteError(int result, const std::string& operation) {
        if (result != SQLITE_OK && result != SQLITE_DONE && result != SQLITE_ROW) {
            std::string errorMsg = operation + ": " + sqlite3_errmsg(db);
            throw std::runtime_error(errorMsg);
        }
    }

public:
    // Constructor with database connection and mappers
    SqliteRepository(
        sqlite3* database,
        const std::string& table,
        std::function<T(sqlite3_stmt*)> mapper,
        std::string columnName,
        bool takeOwnership = false
    ) : db(database), tableName(table), rowMapper(mapper), colName(columnName), ownsDb(takeOwnership) {}

    ~SqliteRepository() {
        if (ownsDb && db) {
            sqlite3_close(db);
        }
    }

    // Delete copy constructor and assignment
    SqliteRepository(const SqliteRepository&) = delete;
    SqliteRepository& operator=(const SqliteRepository&) = delete;

    std::optional<T> get(const std::string& id) override {
        try {
            std::string sql = "SELECT * FROM " + tableName + " WHERE " + colName + " = ?";
            sqlite3_stmt* stmt;
    
            int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
            checkSqliteError(result, "prepare statement");
    
            result = sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);
            checkSqliteError(result, "bind parameter");
    
            std::optional<T> returnValue = std::nullopt;
    
            result = sqlite3_step(stmt);
            if (result == SQLITE_ROW) {
                returnValue = rowMapper(stmt);
            } else if (result != SQLITE_DONE) {
                checkSqliteError(result, "execute query");
            }
    
            sqlite3_finalize(stmt);
            return returnValue;
        } 
        catch (...) {
            throw RepositoryException("Database error");
        }
    }

    // Helper method to insert data
    void insert(const T& item, std::function<void(sqlite3_stmt*, const T&)> binder) {
        std::string sql = "INSERT INTO " + tableName + " VALUES (?, ?, ?, ?)";
        sqlite3_stmt* stmt;

        int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        checkSqliteError(result, "prepare insert");

        binder(stmt, item);

        result = sqlite3_step(stmt);
        checkSqliteError(result, "execute insert");

        sqlite3_finalize(stmt);
    }
};
