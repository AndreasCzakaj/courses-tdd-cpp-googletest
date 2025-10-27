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


static const std::string existingUsersId = "user-1";
static const std::string validEmail = "valid@test.com";
static const std::string validButUnknownEmail = "valid2@test.com";
static const std::string validPassword = "aB.456789012";
static const std::string validButWrongPassword = "aB.4567890123";
static const Credentials validCredentials = {validEmail, validPassword};
static const Credentials invalidCredentials = {"", ""};
static PersonRepository* repo = new VectorRepository<Person>(
        [](const Person& p, const std::string& email) { return p.email == email; },
        {
            {existingUsersId, validEmail, validPassword, "active"}
        }
);

class LoginServiceTest : public ::testing::Test {
protected:
    std::shared_ptr<LoginService> service;

    void SetUp() override {
        service = std::make_shared<LoginService>(repo);
    }
};

TEST_F(LoginServiceTest, LoginWithInvalidCredentialsThrows) {
    auto action = [this] { service->login(invalidCredentials); };
    EXPECT_THAT(action, Throws<ValidationException>());
}

TEST_F(LoginServiceTest, LoginWithUnknownEmailThrows) {
    auto action = [this] { service->login({validButUnknownEmail, validPassword}); };
    EXPECT_THAT(action, Throws<LoginException>(Property(&LoginException::what, StrEq("Unknown email"))));
}

TEST_F(LoginServiceTest, LoginWithWrongPasswordThrows) {
    auto action = [this] { service->login({validEmail, validButWrongPassword}); };
    EXPECT_THAT(action, Throws<LoginException>(Property(&LoginException::what, StrEq("Wrong password"))));
}

TEST_F(LoginServiceTest, LoginFailsOnDbError) {
    service = std::make_shared<LoginService>(new ThrowingRepository<Person>()); // Simulate DB error
    auto action = [this] { service->login({validEmail, validPassword}); };
    EXPECT_THAT(action, Throws<ServerException>(Property(&ServerException::what, StrEq("DB error"))));
}


TEST_F(LoginServiceTest, LoginOK) {
    auto actual = service->login({validEmail, validPassword});
    EXPECT_THAT(actual, Field(&Session::userId, StrEq(existingUsersId)));
}


// Mock implementation of PersonRepository interface
class MockPersonRepository : public IRepository<Person> {
public:
    MOCK_METHOD(std::optional<Person>, get, (const std::string& id), (override));
};

TEST_F(LoginServiceTest, LoginFailsOnDbErrorViaMock) {
    // given
    MockPersonRepository mockRepo;
    EXPECT_CALL(mockRepo, get(_)).WillOnce(Throw(RepositoryException("Database connection failed")));
    service = std::make_shared<LoginService>(&mockRepo);

    // when
    auto action = [this] { service->login({validEmail, validPassword}); };

    // then
    EXPECT_THAT(action, Throws<ServerException>(Property(&ServerException::what, StrEq("DB error"))));
}