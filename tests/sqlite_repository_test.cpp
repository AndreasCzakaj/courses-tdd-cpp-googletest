#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "repository.h"
#include "uss.h"
#include <sqlite3.h>

using ::testing::StrEq;
using ::testing::AllOf;
using ::testing::Field;

// ============================================================================
// SQLite Repository Integration Tests
// ============================================================================

class SqliteRepositoryTest : public ::testing::Test {
protected:
    sqlite3* db = nullptr;

    void SetUp() override {
        // Create in-memory database for testing
        int result = sqlite3_open(":memory:", &db);
        ASSERT_EQ(result, SQLITE_OK) << "Failed to open in-memory database";

        // Create Person table
        const char* createTableSql = R"(
            CREATE TABLE persons (
                id TEXT PRIMARY KEY,
                email TEXT NOT NULL,
                passwordHash TEXT NOT NULL,
                status TEXT NOT NULL
            )
        )";

        char* errorMsg = nullptr;
        result = sqlite3_exec(db, createTableSql, nullptr, nullptr, &errorMsg);
        if (result != SQLITE_OK) {
            std::string error = errorMsg;
            sqlite3_free(errorMsg);
            FAIL() << "Failed to create table: " << error;
        }
    }

    void TearDown() override {
        if (db) {
            sqlite3_close(db);
        }
    }

    // Row mapper: converts SQL row to Person
    static Person personRowMapper(sqlite3_stmt* stmt) {
        Person person;
        person.id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        person.email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        person.passwordHash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        person.status = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        return person;
    }

    // Binder: binds Person fields to prepared statement
    static void personBinder(sqlite3_stmt* stmt, const Person& person) {
        sqlite3_bind_text(stmt, 1, person.id.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, person.email.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, person.passwordHash.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, person.status.c_str(), -1, SQLITE_TRANSIENT);
    }
};

TEST_F(SqliteRepositoryTest, ReturnsPersonWhenFound) {
    SqliteRepository<Person> repo(
        db,
        "persons",
        personRowMapper,
        "id"
    );

    // Insert test data
    Person testPerson{"123", "alice@example.com", "hashedpw", "active"};
    repo.insert(testPerson, personBinder);

    // Retrieve by ID
    auto result = repo.get("123");

    ASSERT_TRUE(result.has_value());
    EXPECT_THAT(*result, AllOf(
        Field(&Person::id, StrEq("123")),
        Field(&Person::email, StrEq("alice@example.com")),
        Field(&Person::passwordHash, StrEq("hashedpw")),
        Field(&Person::status, StrEq("active"))
    ));
}

TEST_F(SqliteRepositoryTest, ReturnsPersonWhenFoundByEmail) {
    SqliteRepository<Person> repo(
        db,
        "persons",
        personRowMapper,
        "email"
    );

    // Insert test data
    Person testPerson{"123", "alice@example.com", "hashedpw", "active"};
    repo.insert(testPerson, personBinder);

    // Retrieve by ID
    auto result = repo.get("alice@example.com");

    ASSERT_TRUE(result.has_value());
    EXPECT_THAT(*result, AllOf(
        Field(&Person::id, StrEq("123")),
        Field(&Person::email, StrEq("alice@example.com")),
        Field(&Person::passwordHash, StrEq("hashedpw")),
        Field(&Person::status, StrEq("active"))
    ));
}

TEST_F(SqliteRepositoryTest, ReturnsNulloptWhenNotFound) {
    SqliteRepository<Person> repo(
        db,
        "persons",
        personRowMapper,
        "id"
    );

    auto result = repo.get("nonexistent");

    EXPECT_FALSE(result.has_value());
}

TEST_F(SqliteRepositoryTest, HandlesMultipleRecords) {
    SqliteRepository<Person> repo(
        db,
        "persons",
        personRowMapper,
        "id"
    );

    // Insert multiple persons
    Person alice{"1", "alice@example.com", "hash1", "active"};
    Person bob{"2", "bob@example.com", "hash2", "inactive"};
    Person charlie{"3", "charlie@example.com", "hash3", "active"};

    repo.insert(alice, personBinder);
    repo.insert(bob, personBinder);
    repo.insert(charlie, personBinder);

    // Retrieve each one
    auto result1 = repo.get("1");
    auto result2 = repo.get("2");
    auto result3 = repo.get("3");

    ASSERT_TRUE(result1.has_value());
    EXPECT_THAT(result1->email, StrEq("alice@example.com"));

    ASSERT_TRUE(result2.has_value());
    EXPECT_THAT(result2->email, StrEq("bob@example.com"));
    EXPECT_THAT(result2->status, StrEq("inactive"));

    ASSERT_TRUE(result3.has_value());
    EXPECT_THAT(result3->email, StrEq("charlie@example.com"));
}

TEST_F(SqliteRepositoryTest, WorksWithRealDatabaseOperations) {
    SqliteRepository<Person> repo(
        db,
        "persons",
        personRowMapper,
        "id"
    );

    // Test that database persists data across operations
    Person user{"user-123", "user@test.com", "securehash", "pending"};
    repo.insert(user, personBinder);

    // First retrieval
    auto firstGet = repo.get("user-123");
    ASSERT_TRUE(firstGet.has_value());
    EXPECT_THAT(firstGet->status, StrEq("pending"));

    // Second retrieval - should get same data
    auto secondGet = repo.get("user-123");
    ASSERT_TRUE(secondGet.has_value());
    EXPECT_THAT(secondGet->email, StrEq("user@test.com"));
}

TEST_F(SqliteRepositoryTest, ThrowsOnInvalidTable) {
    SqliteRepository<Person> repo(
        db,
        "nonexistent_table",
        personRowMapper,
        "id"
    );

    EXPECT_THROW(repo.get("123"), RepositoryException);
}

TEST_F(SqliteRepositoryTest, HandlesSpecialCharactersInData) {
    SqliteRepository<Person> repo(
        db,
        "persons",
        personRowMapper,
        "id"
    );

    Person specialUser{
        "id-with-dashes",
        "user+tag@example.com",
        "hash$with$special",
        "active"
    };

    repo.insert(specialUser, personBinder);

    auto result = repo.get("id-with-dashes");
    ASSERT_TRUE(result.has_value());
    EXPECT_THAT(result->email, StrEq("user+tag@example.com"));
    EXPECT_THAT(result->passwordHash, StrEq("hash$with$special"));
}
