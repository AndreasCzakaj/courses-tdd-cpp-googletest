#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "repository.h"
#include "uss.h"

using ::testing::StrEq;
using ::testing::AllOf;
using ::testing::Field;

// ============================================================================
// VectorRepository Tests
// ============================================================================

struct Book {
    std::string isbn;
    std::string title;
};

static const auto filterByIsbn = [](const Book& book, const std::string& isbn) {
        return book.isbn == isbn;
};

static const std::string existingIsbn = "123465789";

static const std::vector<Book> initialData = {
        {existingIsbn, "Necronomicon"},
};


TEST(VectorRepositoryTest, ReturnsItemWhenFound) {
    VectorRepository<Book> repo(filterByIsbn, initialData);

    auto result = repo.get(existingIsbn);
    ASSERT_TRUE(result.has_value());
    EXPECT_THAT(*result, AllOf(
        Field(&Book::isbn, StrEq(existingIsbn)),
        Field(&Book::title, StrEq("Necronomicon"))
    ));
}

TEST(VectorRepositoryTest, ReturnsNulloptWhenNotFound) {
    VectorRepository<Book> repo(filterByIsbn, initialData);

    auto result = repo.get("999");
    EXPECT_FALSE(result.has_value());
}