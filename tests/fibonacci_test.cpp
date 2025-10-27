#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "fibonacci.h"
#include <memory>
#include <vector>
#include <string>

using ::testing::IsEmpty;
using ::testing::Not;
using ::testing::Eq;
using ::testing::StrEq;
using ::testing::Throws;

// ============================================================================
// Parameterized test fixture to test ALL implementations
// ============================================================================

class FibonacciTest : public ::testing::TestWithParam<std::shared_ptr<IFibonacci>> {
protected:
    std::shared_ptr<IFibonacci> fib;

    void SetUp() override {
        fib = GetParam();
    }
};

// Factory functions to create implementations
std::shared_ptr<IFibonacci> createLoopImpl() {
    return std::make_shared<FibonacciLoopImpl>();
}

std::shared_ptr<IFibonacci> createRecursionImpl() {
    return std::make_shared<FibonacciRecursionImpl>();
}

// Instantiate tests for BOTH implementations
INSTANTIATE_TEST_SUITE_P(
    AllImplementations,
    FibonacciTest,
    ::testing::Values(
        createLoopImpl(),
        createRecursionImpl()
    ),
    [](const ::testing::TestParamInfo<std::shared_ptr<IFibonacci>>& info) {
        // Name the test instances based on implementation type
        if (dynamic_cast<FibonacciLoopImpl*>(info.param.get())) {
            return std::string("LoopImpl");
        } else if (dynamic_cast<FibonacciRecursionImpl*>(info.param.get())) {
            return std::string("RecursionImpl");
        }
        return std::string("Unknown");
    }
);

// ============================================================================
// Test base cases
// ============================================================================

TEST_P(FibonacciTest, ReturnsZeroForIndexZero) {
    EXPECT_THAT(fib->calc(0), Eq(0));
}

TEST_P(FibonacciTest, ReturnsOneForIndexOne) {
    EXPECT_THAT(fib->calc(1), Eq(1));
}

// ============================================================================
// Test small values
// ============================================================================

TEST_P(FibonacciTest, ReturnsCorrectValueForSmallIndices) {
    EXPECT_EQ(fib->calc(2), 1);
    EXPECT_EQ(fib->calc(3), 2);
    EXPECT_EQ(fib->calc(4), 3);
    EXPECT_EQ(fib->calc(5), 5);
    EXPECT_EQ(fib->calc(6), 8);
    EXPECT_EQ(fib->calc(7), 13);
    EXPECT_EQ(fib->calc(8), 21);
    EXPECT_EQ(fib->calc(9), 34);
    EXPECT_EQ(fib->calc(10), 55);
}

// ============================================================================
// Test medium values
// ============================================================================

TEST_P(FibonacciTest, ReturnsCorrectValueForMediumIndices) {
    EXPECT_EQ(fib->calc(15), 610);
    EXPECT_EQ(fib->calc(20), 6765);
    EXPECT_EQ(fib->calc(25), 75025);
    EXPECT_EQ(fib->calc(30), 832040);
}

// ============================================================================
// Test boundary values - maximum valid index
// ============================================================================

TEST_P(FibonacciTest, DISABLED_ReturnsCorrectValueForIndex46) {
    EXPECT_EQ(fib->calc(46), 1836311903);
}

TEST_P(FibonacciTest, DISABLED_ReturnsCorrectValueForIndex45) {
    EXPECT_EQ(fib->calc(45), 1134903170);
}

// ============================================================================
// Test error cases - negative indices
// ============================================================================

TEST_P(FibonacciTest, ThrowsExceptionForNegativeOne) {
    EXPECT_THROW({
        fib->calc(-1);
    }, std::out_of_range);
}

TEST_P(FibonacciTest, ThrowsExceptionForNegativeValues) {
    EXPECT_THROW(fib->calc(-5), std::out_of_range);
    EXPECT_THROW(fib->calc(-100), std::out_of_range);
    EXPECT_THROW(fib->calc(-2147483648), std::out_of_range); // INT_MIN
}

// ============================================================================
// Test error cases - indices too large
// ============================================================================

TEST_P(FibonacciTest, ThrowsExceptionForIndex47) {
    EXPECT_THROW({
        fib->calc(47);
    }, std::out_of_range);
}

TEST_P(FibonacciTest, ThrowsExceptionForLargeIndices) {
    EXPECT_THROW(fib->calc(48), std::out_of_range);
    EXPECT_THROW(fib->calc(50), std::out_of_range);
    EXPECT_THROW(fib->calc(100), std::out_of_range);
    EXPECT_THROW(fib->calc(2147483647), std::out_of_range); // INT_MAX
}

// ============================================================================
// Test exception message
// ============================================================================

// Note: Google Test doesn't have built-in message matchers for EXPECT_THROW,
// so we need try-catch to verify the message content
TEST_P(FibonacciTest, ExceptionContainsDescriptiveMessage) {
    try {
        fib->calc(-1);
        FAIL() << "Expected std::out_of_range to be thrown";
    } catch (const std::out_of_range& e) {
        EXPECT_STREQ(e.what(), "Index must be between 0 and 46");
    } catch (...) {
        FAIL() << "Expected std::out_of_range";
    }
}

TEST_P(FibonacciTest, ExceptionContainsDescriptiveMessage2) {
    auto action = [this] { fib->calc(-1); };
    EXPECT_THAT(
        action,
        Throws<std::out_of_range>(
            Property(&std::out_of_range::what, StrEq("Index must be between 0 and 46"))
        )
    ) << "It should throw out_of_range with descriptive message";
}

// Pragmatic alternative: Separate tests for "throws" vs "message"
// This keeps exception-type tests flat, message test has try-catch
TEST_P(FibonacciTest, NegativeIndexThrowsCorrectExceptionType) {
    EXPECT_THROW(fib->calc(-1), std::out_of_range);  // Flat!
}

TEST_P(FibonacciTest, ExceptionHasCorrectMessage) {
    // Only test message content if it's important for your use case
    try {
        fib->calc(47);
        FAIL() << "Should have thrown";
    } catch (const std::out_of_range& e) {
        EXPECT_STREQ(e.what(), "Index must be between 0 and 46");
    }
}

// ============================================================================
// Additional parameterized tests for valid values (tests both implementations)
// ============================================================================

using ValidTestParams = std::tuple<std::shared_ptr<IFibonacci>, int, int64_t>;

class ValidFibonacciTest : public ::testing::TestWithParam<ValidTestParams> {};

TEST_P(ValidFibonacciTest, CalculatesCorrectValues) {
    auto [fib, index, expected] = GetParam();
    EXPECT_EQ(fib->calc(index), expected);
}

// Helper to generate test values for both implementations
std::vector<ValidTestParams> generateValidTestParams() {
    std::vector<std::pair<int, int64_t>> testData = {
        {0, 0}, {1, 1}, {2, 1}, {3, 2}, {4, 3}, {5, 5},
        {10, 55}, {15, 610}, {20, 6765}, {30, 832040}/* ,
        {40, 102334155}, {46, 1836311903} */
    };

    std::vector<ValidTestParams> result;
    for (const auto& [index, expected] : testData) {
        result.push_back(std::make_tuple(createLoopImpl(), index, expected));
        result.push_back(std::make_tuple(createRecursionImpl(), index, expected));
    }
    return result;
}

INSTANTIATE_TEST_SUITE_P(
    KnownValuesAllImpls,
    ValidFibonacciTest,
    ::testing::ValuesIn(generateValidTestParams()),
    [](const ::testing::TestParamInfo<ValidTestParams>& info) {
        auto fib = std::get<0>(info.param);
        auto index = std::get<1>(info.param);
        std::string impl_name = dynamic_cast<FibonacciLoopImpl*>(fib.get()) ? "Loop" : "Recursion";
        return impl_name + "_Index" + std::to_string(index);
    }
);

// ============================================================================
// Additional parameterized tests for invalid indices (tests both implementations)
// ============================================================================

using InvalidTestParams = std::tuple<std::shared_ptr<IFibonacci>, int>;

class InvalidFibonacciTest : public ::testing::TestWithParam<InvalidTestParams> {};

TEST_P(InvalidFibonacciTest, ThrowsOutOfRangeException) {
    auto [fib, invalid_index] = GetParam();
    EXPECT_THROW(fib->calc(invalid_index), std::out_of_range);
}

// Helper to generate invalid test values for both implementations
std::vector<InvalidTestParams> generateInvalidTestParams() {
    std::vector<int> invalidIndices = {-1, -2, -10, -100, 47, 48, 50, 100, 1000};

    std::vector<InvalidTestParams> result;
    for (int index : invalidIndices) {
        result.push_back(std::make_tuple(createLoopImpl(), index));
        result.push_back(std::make_tuple(createRecursionImpl(), index));
    }
    return result;
}

INSTANTIATE_TEST_SUITE_P(
    InvalidIndicesAllImpls,
    InvalidFibonacciTest,
    ::testing::ValuesIn(generateInvalidTestParams()),
    [](const ::testing::TestParamInfo<InvalidTestParams>& info) {
        auto fib = std::get<0>(info.param);
        auto index = std::get<1>(info.param);
        std::string impl_name = dynamic_cast<FibonacciLoopImpl*>(fib.get()) ? "Loop" : "Recursion";
        std::string index_str = std::to_string(index);
        // Replace negative sign with 'Neg' for valid test names
        if (index < 0) {
            index_str = "Neg" + std::to_string(-index);
        }
        return impl_name + "_Index" + index_str;
    }
);
