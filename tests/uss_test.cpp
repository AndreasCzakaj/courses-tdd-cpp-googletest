#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "uss.h"

using ::testing::Eq;
using ::testing::StrEq;
using ::testing::Throws;
using ::testing::Property;
using ::testing::Field;
using ::testing::AllOf;
using ::testing::ElementsAre;

// ============================================================================
// Valid email test cases
// ============================================================================

struct ValidEmailTestCase {
    std::string input;
    std::string expectedOutput;
};

class ValidEmailTest : public ::testing::TestWithParam<ValidEmailTestCase> {};

TEST_P(ValidEmailTest, SanitizesAndReturnsEmail) {
    auto testCase = GetParam();
    EXPECT_THAT(sanitizeAndValidateEmail(testCase.input), StrEq(testCase.expectedOutput));
}

INSTANTIATE_TEST_SUITE_P(
    ValidEmails,
    ValidEmailTest,
    ::testing::Values(
        ValidEmailTestCase{"test@example.com", "test@example.com"},
        ValidEmailTestCase{"  test@example.com  ", "test@example.com"},
        ValidEmailTestCase{"TEST@EXAMPLE.COM", "test@example.com"},
        ValidEmailTestCase{"  TEST@EXAMPLE.COM  ", "test@example.com"},
        ValidEmailTestCase{"user.name@example.com", "user.name@example.com"},
        ValidEmailTestCase{"user-123@test.com", "user-123@test.com"}
    )
);

// ============================================================================
// Invalid email test cases
// ============================================================================

struct InvalidEmailTestCase {
    std::string input;
    std::string expectedErrorMessage;
};

class InvalidEmailTest : public ::testing::TestWithParam<InvalidEmailTestCase> {};

TEST_P(InvalidEmailTest, DISABLED_ThrowsExceptionWithMessage) {
    auto testCase = GetParam();
    auto action = [&testCase] { sanitizeAndValidateEmail(testCase.input); };
    EXPECT_THAT(
        action,
        Throws<std::invalid_argument>(
            Property(&std::invalid_argument::what, StrEq(testCase.expectedErrorMessage))
        )
    );
}

INSTANTIATE_TEST_SUITE_P(
    InvalidEmails,
    InvalidEmailTest,
    ::testing::Values(
        // Too short
        InvalidEmailTestCase{"a@b.c", "Email must be at least 6 characters"}

        // Missing @ symbol
        // ...

        // Missing . symbol
        // ...

        // Invalid characters
        // ...
    )
);

// ============================================================================
// Valid password test cases
// ============================================================================

struct ValidPasswordTestCase {
    std::string input;
    std::string expectedOutput;
};

class ValidPasswordTest : public ::testing::TestWithParam<ValidPasswordTestCase> {};

TEST_P(ValidPasswordTest, DISABLED_SanitizesAndReturnsPassword) {
    auto testCase = GetParam();
    EXPECT_THAT(sanitizeAndValidatePassword(testCase.input), StrEq(testCase.expectedOutput));
}

INSTANTIATE_TEST_SUITE_P(
    ValidPasswords,
    ValidPasswordTest,
    ::testing::Values(
        ValidPasswordTestCase{"aB.456789012", "aB.456789012"}
        // ...
    )
);

// ============================================================================
// Invalid password test cases
// ============================================================================

struct InvalidPasswordTestCase {
    std::string input;
    std::string expectedErrorMessage;
};

class InvalidPasswordTest : public ::testing::TestWithParam<InvalidPasswordTestCase> {};

TEST_P(InvalidPasswordTest, DISABLED_ThrowsExceptionWithMessage) {
    auto testCase = GetParam();
    auto action = [&testCase] { sanitizeAndValidatePassword(testCase.input); };
    EXPECT_THAT(
        action,
        Throws<std::invalid_argument>(
            Property(&std::invalid_argument::what, StrEq(testCase.expectedErrorMessage))
        )
    );
}

INSTANTIATE_TEST_SUITE_P(
    InvalidPasswords,
    InvalidPasswordTest,
    ::testing::Values(
        // Too short
        InvalidPasswordTestCase{"aB.45678901", "Password must be at least 12 characters"}
        // ...

        // Too long
        // ...

        // Missing lowercase
        // ...

        // Missing uppercase
        // ...

        // Missing digit
        // ...

        // Missing special character
        // ...

        // Invalid characters
        // ...
    )
);

// ============================================================================
// Valid credentials test cases
// ============================================================================

struct ValidCredentialsTestCase {
    Credentials input;
    Credentials expectedOutput;
};

class ValidCredentialsTest : public ::testing::TestWithParam<ValidCredentialsTestCase> {};

TEST_P(ValidCredentialsTest, DISABLED_SanitizesAndReturnsCredentials) {
    auto testCase = GetParam();
    auto result = sanitizeAndValidateCredentials(testCase.input);
    EXPECT_THAT(result.email, StrEq(testCase.expectedOutput.email));
    EXPECT_THAT(result.plainPassword, StrEq(testCase.expectedOutput.plainPassword));
}

INSTANTIATE_TEST_SUITE_P(
    ValidCredentials,
    ValidCredentialsTest,
    ::testing::Values(
        ValidCredentialsTestCase{
            {"  TEST@EXAMPLE.COM  ", "  Passw0rd.123  "},
            {"test@example.com", "Passw0rd.123"}
        }
    )
);

// ============================================================================
// Invalid credentials test cases - Single field errors
// ============================================================================

struct InvalidCredentialsSingleErrorTestCase {
    Credentials input;
    std::string expectedField;
    std::string expectedMessage;
};

class InvalidCredentialsSingleErrorTest : public ::testing::TestWithParam<InvalidCredentialsSingleErrorTestCase> {};

TEST_P(InvalidCredentialsSingleErrorTest, DISABLED_ThrowsValidationExceptionWithSingleError) {
    auto testCase = GetParam();
    auto action = [&testCase] { sanitizeAndValidateCredentials(testCase.input); };

    EXPECT_THAT(
        action,
        Throws<ValidationException>(
            Property(&ValidationException::getErrors,
                ElementsAre(
                    AllOf(
                        Field(&ValidationError::field, StrEq(testCase.expectedField)),
                        Field(&ValidationError::message, StrEq(testCase.expectedMessage))
                    )
                )
            )
        )
    );
}

INSTANTIATE_TEST_SUITE_P(
    InvalidCredentialsSingleError,
    InvalidCredentialsSingleErrorTest,
    ::testing::Values(
        // Invalid email only
        InvalidCredentialsSingleErrorTestCase{
            {"invalid-email", "Passw0rd.123"},
            "email", "Email must contain @"
        },
        // Invalid password only
        InvalidCredentialsSingleErrorTestCase{
            {"test@example.com", "short"},
            "plainPassword", "Password must be at least 12 characters"
        }
    )
);

// ============================================================================
// Invalid credentials test cases - Both fields invalid
// ============================================================================

struct InvalidCredentialsBothErrorsTestCase {
    Credentials input;
    std::string expectedEmailError;
    std::string expectedPasswordError;
};

class InvalidCredentialsBothErrorsTest : public ::testing::TestWithParam<InvalidCredentialsBothErrorsTestCase> {};

TEST_P(InvalidCredentialsBothErrorsTest, DISABLED_ThrowsValidationExceptionWithBothErrors) {
    auto testCase = GetParam();
    auto action = [&testCase] { sanitizeAndValidateCredentials(testCase.input); };

    EXPECT_THAT(
        action,
        Throws<ValidationException>(
            Property(&ValidationException::getErrors,
                ElementsAre(
                    AllOf(
                        Field(&ValidationError::field, StrEq("email")),
                        Field(&ValidationError::message, StrEq(testCase.expectedEmailError))
                    ),
                    AllOf(
                        Field(&ValidationError::field, StrEq("plainPassword")),
                        Field(&ValidationError::message, StrEq(testCase.expectedPasswordError))
                    )
                )
            )
        )
    );
}

INSTANTIATE_TEST_SUITE_P(
    InvalidCredentialsBothErrors,
    InvalidCredentialsBothErrorsTest,
    ::testing::Values(
        // Both invalid
        InvalidCredentialsBothErrorsTestCase{
            {"invalid", "short"},
            "Email must contain @",
            "Password must be at least 12 characters"
        }
    )
);

