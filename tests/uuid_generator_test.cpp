#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "uuid_generator.h"
#include <regex>
#include <memory>

using ::testing::MatchesRegex;

// ============================================================================
// UUID Generator Tests - Parameterized
// ============================================================================

struct UuidGeneratorTestCase {
    std::shared_ptr<UuidGenerator> generator;
    std::string expectedRegex;
    std::string description;
};

class UuidGeneratorTest : public ::testing::TestWithParam<UuidGeneratorTestCase> {};

TEST_P(UuidGeneratorTest, ShouldCreateAUuidInTheMatchingFormat) {
    auto testCase = GetParam();

    // when
    auto actual = testCase.generator->create();

    // then
    EXPECT_THAT(actual, MatchesRegex(testCase.expectedRegex))
        << "Description: " << testCase.description;
}

INSTANTIATE_TEST_SUITE_P(
    UuidFormatValidation,
    UuidGeneratorTest,
    ::testing::Values(
        UuidGeneratorTestCase{
            std::make_shared<UuidGeneratorNaiveRandomImpl>(),
            "[a-f0-9]{32}",
            "lower case, no dashes"
        }
        // UuidGeneratorTestCase{
        //     ???,
        //     "[A-F0-9]{32}",
        //     "upper case, no dashes"
        // },
        // UuidGeneratorTestCase{
        //     ???,
        //     "[a-f0-9]{8}-[a-f0-9]{4}-[a-f0-9]{4}-[a-f0-9]{4}-[a-f0-9]{12}",
        //     "lower case, with dashes"
        // },
        // UuidGeneratorTestCase{
        //     ???,
        //     "[A-F0-9]{8}-[A-F0-9]{4}-[A-F0-9]{4}-[A-F0-9]{4}-[A-F0-9]{12}",
        //     "upper case, with dashes"
        // }
    ),
    [](const ::testing::TestParamInfo<UuidGeneratorTestCase>& info) {
        // Create test name from description
        std::string name = info.param.description;
        // Replace spaces and special chars with underscores
        std::replace_if(name.begin(), name.end(),
            [](char c) { return !std::isalnum(c); }, '_');
        return name;
    }
);

// ============================================================================
// Additional tests to verify randomness
// ============================================================================

TEST(UuidGeneratorTest, CreateGeneratesDifferentValues) {
    UuidGeneratorNaiveRandomImpl generator;

    auto uuid1 = generator.create();
    auto uuid2 = generator.create();

    EXPECT_NE(uuid1, uuid2) << "UUIDs should be different on each call";
}

TEST(UuidGeneratorTest, CreateGeneratesCorrectLength) {
    UuidGeneratorNaiveRandomImpl generator;

    auto uuid = generator.create();

    EXPECT_EQ(uuid.length(), 32) << "UUID should be exactly 32 characters";
}

TEST(UuidGeneratorTest, CreateGeneratesOnlyHexCharacters) {
    UuidGeneratorNaiveRandomImpl generator;

    auto uuid = generator.create();

    EXPECT_THAT(uuid, MatchesRegex("^[a-f0-9]+$"))
        << "UUID should contain only lowercase hex characters";
}
