#include <gtest/gtest.h>
#include "fibonacci.h"

// https://www.wackerart.de/mathematik/big_numbers/fibonacci_numbers.html

// ============================================================================
// Basic test - this will fail until implemented
// ============================================================================

TEST(FibonacciTest, ReturnsZeroForIndexZero) {
    Fibonacci fib;
    EXPECT_EQ(fib.calc(0), 0);
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
