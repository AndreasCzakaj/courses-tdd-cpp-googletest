#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "fibonacci.h"

using ::testing::IsEmpty;
using ::testing::Not;
using ::testing::Eq;

// https://www.wackerart.de/mathematik/big_numbers/fibonacci_numbers.html

// ============================================================================
// Basic test - this will fail until implemented
// ============================================================================

TEST(FibonacciTest,  /* DISABLED_ */ReturnZeroForIndexZero) {
    // given / Arrange
    Fibonacci fib;
    int index = 0;
    
    // when / Act
    auto actual = fib.calc(index);
    
    // then / Assert
    int expected = 0;
    EXPECT_EQ(actual, expected) << "It should yield zero for index zero I";
    EXPECT_THAT(actual, Eq(expected)) << "It should yield zero for index zero II";
    
    // this is the same but compacted:
    EXPECT_EQ(fib.calc(0), 0) << "It should yield zero for index zero III";
    
    // this is the same but different: will STOP the test on failure:
    ASSERT_EQ(fib.calc(0), 0) << "It should yield zero for index zero IV";
}

TEST(FibonacciTest, DISABLED_ReturnsOneForIndexOne) {
}

TEST(FibonacciTest, DISABLED_ReturnsOneForIndexTwo) {
}

TEST(FibonacciTest, DISABLED_ReturnsTwoForIndexThree) {
}

TEST(FibonacciTest, DISABLED_ReturnsThreeForIndexFour) {
}

TEST(FibonacciTest, DISABLED_ReturnsFiveForIndexFive) {
}

TEST(FibonacciTest, DISABLED_ReturnsEightForIndexSix) {
}

TEST(FibonacciTest, DISABLED_ReturnsCorrectValueForSmallIndices) {
}

TEST(FibonacciTest, DISABLED_ReturnsCorrectValueForLargeIndices) {
}

TEST(FibonacciTest, DISABLED_ThrowsExceptionForNegativeIndices) {
}

TEST(FibonacciTest, DISABLED_ThrowsExceptionForIndicesTooLarge) {
}
