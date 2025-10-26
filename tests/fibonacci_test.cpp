#include <gtest/gtest.h>
#include "fibonacci.h"

// https://www.wackerart.de/mathematik/big_numbers/fibonacci_numbers.html

// ============================================================================
// Basic test - this will fail until implemented
// ============================================================================

TEST(FibonacciTest,  DISABLED_oForIndexZero) {
    // given / Arrange
    Fibonacci fib;
    int index = 0;

    // when / Act
    auto actual = fib.calc(index);

    // then / Assert
    int expected = 0;
    EXPECT_EQ(actual, expected);

    // this is the same but compacted:
    EXPECT_EQ(fib.calc(0), 0);

    // this is the same but different: will STOP the test on failure:
    ASSERT_EQ(fib.calc(0), 0);
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

TEST(FibonacciTest, DISABLED_ReturnsCorrectValueForMediumIndices) {
}

TEST(FibonacciTest, DISABLED_ReturnsCorrectValueForLargeIndices) {
}

TEST(FibonacciTest, DISABLED_ThrowsExceptionForNegativeIndices) {
}

TEST(FibonacciTest, DISABLED_ThrowsExceptionForIndicesTooLarge) {
}

TEST(FibonacciTest, DISABLED_ExceptionHasDescriptiveMessage) {
}
