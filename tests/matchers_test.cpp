#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <vector>
#include <set>
#include <nlohmann/json.hpp>

using ::testing::IsEmpty;
using ::testing::Not;


std::string email = "andreas.czakaj@binary-stars.eu";

TEST(MatchersTest, DISABLED_Email) {
    EXPECT_THAT(email, IsEmpty()) << "Email should not be empty";
}


std::vector<std::string> list = {"a", "b", "c"};

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


Person loadPerson() {
    throw std::runtime_error("oops");
}