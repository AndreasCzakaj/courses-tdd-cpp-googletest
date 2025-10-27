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

TEST_P(InvalidEmailTest, ThrowsExceptionWithMessage) {
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
        InvalidEmailTestCase{"a@b.c", "Email must be at least 6 characters"},
        InvalidEmailTestCase{"test", "Email must be at least 6 characters"},

        // Missing @ symbol
        InvalidEmailTestCase{"testexample.com", "Email must contain @"},
        InvalidEmailTestCase{"user.domain.com", "Email must contain @"},

        // Missing . symbol
        InvalidEmailTestCase{"test@examplecom", "Email must contain ."},
        InvalidEmailTestCase{"user@domain", "Email must contain ."},

        // Invalid characters
        InvalidEmailTestCase{"test@exam ple.com", "Email contains invalid characters"},
        InvalidEmailTestCase{"test#example.com", "Email must contain @"},
        InvalidEmailTestCase{"test@example!.com", "Email contains invalid characters"},
        InvalidEmailTestCase{"user@domain$.com", "Email contains invalid characters"}
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

TEST_P(ValidPasswordTest, SanitizesAndReturnsPassword) {
    auto testCase = GetParam();
    EXPECT_THAT(sanitizeAndValidatePassword(testCase.input), StrEq(testCase.expectedOutput));
}

INSTANTIATE_TEST_SUITE_P(
    ValidPasswords,
    ValidPasswordTest,
    ::testing::Values(
        ValidPasswordTestCase{"Passw0rd.123", "Passw0rd.123"},
        ValidPasswordTestCase{"  Passw0rd.123  ", "Passw0rd.123"},
        ValidPasswordTestCase{"Ab1.cdefghijk", "Ab1.cdefghijk"},
        ValidPasswordTestCase{"MyP4ss-word_2024", "MyP4ss-word_2024"},
        ValidPasswordTestCase{"Test1234+Valid,Pass", "Test1234+Valid,Pass"},
        ValidPasswordTestCase{"Secure:Pass;123", "Secure:Pass;123"},
        ValidPasswordTestCase{"MaxLength1234567890aB.", "MaxLength1234567890aB."}
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

TEST_P(InvalidPasswordTest, ThrowsExceptionWithMessage) {
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
        InvalidPasswordTestCase{"Pass1.a", "Password must be at least 12 characters"},
        InvalidPasswordTestCase{"Ab1.defgh", "Password must be at least 12 characters"},

        // Too long
        InvalidPasswordTestCase{"ThisPasswordIsWayTooLong1.", "Password must be at most 24 characters"},
        InvalidPasswordTestCase{"AbCdEfGh1234567890.+-_,;:Extra", "Password must be at most 24 characters"},

        // Missing lowercase
        InvalidPasswordTestCase{"PASSWORD123.", "Password must contain at least one lowercase letter"},
        InvalidPasswordTestCase{"HELLO1234-WORLD", "Password must contain at least one lowercase letter"},

        // Missing uppercase
        InvalidPasswordTestCase{"password123.", "Password must contain at least one uppercase letter"},
        InvalidPasswordTestCase{"hello1234-world", "Password must contain at least one uppercase letter"},

        // Missing digit
        InvalidPasswordTestCase{"Password.abc", "Password must contain at least one digit"},
        InvalidPasswordTestCase{"HelloWorld-Test.", "Password must contain at least one digit"},

        // Missing special character
        InvalidPasswordTestCase{"Password1234", "Password must contain at least one special character"},
        InvalidPasswordTestCase{"HelloWorld123", "Password must contain at least one special character"},

        // Invalid characters
        InvalidPasswordTestCase{"Pass word123.", "Password contains invalid characters"},
        InvalidPasswordTestCase{"Password123!", "Password contains invalid characters"},
        InvalidPasswordTestCase{"Pass@word123", "Password contains invalid characters"},
        InvalidPasswordTestCase{"Password#123", "Password contains invalid characters"}
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

TEST_P(ValidCredentialsTest, SanitizesAndReturnsCredentials) {
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

TEST_P(InvalidCredentialsSingleErrorTest, ThrowsValidationExceptionWithSingleError) {
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

TEST_P(InvalidCredentialsBothErrorsTest, ThrowsValidationExceptionWithBothErrors) {
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

