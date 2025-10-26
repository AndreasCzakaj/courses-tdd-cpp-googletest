#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <vector>
#include <set>
#include <nlohmann/json.hpp>

using ::testing::IsEmpty;
using ::testing::Not;
using ::testing::Contains;
using ::testing::AllOf;
using ::testing::SizeIs;
using ::testing::StrEq;
using ::testing::StartsWith;
using ::testing::EndsWith;
using ::testing::HasSubstr;
using ::testing::MatchesRegex;
using ::testing::Field;
using ::testing::FieldsAre;
using ::testing::Throws;
using ::testing::Property;
using ::testing::StrEq;

std::string email = "andreas.czakaj@binary-stars.eu";

TEST(MatchersTest, DISABLED_Email) {
    EXPECT_THAT(email, IsEmpty()) << "Email should not be empty";
}

TEST(MatchersTest, EmailIsExactValue) {
    EXPECT_THAT(email, StrEq("andreas.czakaj@binary-stars.eu"));
}

TEST(MatchersTest, EmailStartsWithAndreas) {
    EXPECT_THAT(email, StartsWith("andreas"));
}

TEST(MatchersTest, EmailEndsWithDotEu) {
    EXPECT_THAT(email, EndsWith(".eu"));
}

TEST(MatchersTest, EmailDoesNotEndWithDotCom) {
    EXPECT_THAT(email, Not(EndsWith(".com")));
}

TEST(MatchersTest, EmailContainsBinary) {
    EXPECT_THAT(email, HasSubstr("binary"));
}

TEST(MatchersTest, EmailMatchesRegex) {
    EXPECT_THAT(email, MatchesRegex("[a-z.@\\-]+"));
}


std::vector<std::string> list = {"a", "b", "c"};

// Custom matcher to check for no duplicates
MATCHER(HasNoDuplicates, "has no duplicate elements") {
    std::set<typename std::remove_reference<decltype(arg)>::type::value_type> seen;
    for (const auto& item : arg) {
        if (seen.count(item) > 0) {
            *result_listener << "found duplicate: " << item;
            return false;
        }
        seen.insert(item);
    }
    return true;
}

TEST(MatchersTest, ListContainsA) {
    EXPECT_THAT(list, Contains("a"));
}

TEST(MatchersTest, ListDoesNotContainD) {
    EXPECT_THAT(list, Not(Contains("d")));
}

TEST(MatchersTest, ListContainsCAndA) {
    EXPECT_THAT(list, AllOf(Contains("c"), Contains("a")));
}

TEST(MatchersTest, ListHasSize3) {
    EXPECT_THAT(list, SizeIs(3));
}

TEST(MatchersTest, ListHasNoDuplicates) {
    std::vector<std::string> list = {"a", "b", "c"};
    EXPECT_THAT(list, HasNoDuplicates());
}

// ============================================================================
// Object Tests using C++ struct and Member Matchers
// ============================================================================

struct Person {
    int id;
    std::string firstName;
    std::string lastName;
    std::string email;
    std::string ipAddress;

    bool operator==(const Person& other) const {
        return id == other.id &&
               firstName == other.firstName &&
               lastName == other.lastName &&
               email == other.email &&
               ipAddress == other.ipAddress;
    }
};

Person person = {1, "Skippy", "Rayne", "srayne0@dot.gov", "229.183.132.150"};


TEST(MatchersTest, ObjectIsEqual) {
    Person person = {1, "Skippy", "Rayne", "srayne0@dot.gov", "229.183.132.150"};
    Person expected = {1, "Skippy", "Rayne", "srayne0@dot.gov", "229.183.132.150"};

    EXPECT_EQ(person, expected);
}

TEST(MatchersTest, ObjectHasSpecificFields) {
    Person person = {1, "Skippy", "Rayne", "srayne0@dot.gov", "229.183.132.150"};

    // Check that firstName and email fields exist and are not empty
    EXPECT_THAT(person, AllOf(
        Field(&Person::firstName, Not(IsEmpty())),
        Field(&Person::email, Not(IsEmpty()))
    ));
}

TEST(MatchersTest, ObjectContainsSubstructure) {
    Person person = {1, "Skippy", "Rayne", "srayne0@dot.gov", "229.183.132.150"};

    // Check that the object contains specific field values (like a subset)
    EXPECT_THAT(person, AllOf(
        Field(&Person::lastName, StrEq("Rayne")),
        Field(&Person::email, StrEq("srayne0@dot.gov"))
    ));
}




Person loadPerson() {
    throw std::runtime_error("oops");
}

TEST(MatchersTest, ItShouldFailHere) {
    auto action = [] { loadPerson(); };
    EXPECT_THAT(
        action,
        Throws<std::runtime_error>(
            Property(&std::runtime_error::what, StrEq("oops"))
        )
    ) << "It should throw runtime_error with 'oops' message";
}