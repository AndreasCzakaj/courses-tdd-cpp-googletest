#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <vector>
#include "login_service.h"
#include "uss.h"
#include "repository.h"

using ::testing::Eq;
using ::testing::StrEq;
using ::testing::Throws;
using ::testing::Property;
using ::testing::Field;
using ::testing::AllOf;
using ::testing::ElementsAre;
using ::testing::Return;
using ::testing::_;
using ::testing::Throw;


static const Credentials invalidCredentials = {"", ""};

/* static PersonRepository* repo = new VectorRepository<Person>(
        [](const Person& p, const std::string& email) { return p.email == email; },
        {
            {existingUsersId, validEmail, validPassword, "active"}
        }
); */

class LoginServiceTest : public ::testing::Test {
protected:
    std::shared_ptr<LoginService> service;

    void SetUp() override {
        service = std::make_shared<LoginService>();
    }
};

TEST_F(LoginServiceTest, DISABLED_LoginWithInvalidCredentialsThrows) {
    auto action = [this] { service->login(invalidCredentials); };
    EXPECT_THAT(action, Throws<ValidationException>());
}

TEST_F(LoginServiceTest, DISABLED_LoginWithUnknownEmailThrows) {
}

TEST_F(LoginServiceTest, DISABLED_LoginWithWrongPasswordThrows) {
}

TEST_F(LoginServiceTest, DISABLED_LoginFailsOnDbError) {
}


TEST_F(LoginServiceTest, DISABLED_LoginOK) {
}


// Mock implementation of PersonRepository interface
class MockPersonRepository : public IRepository<Person> {
public:
    MOCK_METHOD(std::optional<Person>, get, (const std::string& id), (override));
};

TEST_F(LoginServiceTest, DISABLED_LoginFailsOnDbErrorViaMock) {
}