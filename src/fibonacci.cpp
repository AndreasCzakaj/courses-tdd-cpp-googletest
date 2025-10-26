#include "fibonacci.h"

// ============================================================================
// FibonacciLoopImpl - Iterative implementation
// ============================================================================

int64_t FibonacciLoopImpl::calc(int index) const {
    // Validate input range
    if (index < 0 || index > 46) {
        throw std::out_of_range("Index must be between 0 and 46");
    }

    // Base cases
    if (index == 0) return 0;
    if (index == 1) return 1;

    // Iterative calculation to avoid recursion overhead
    int64_t prev2 = 0;
    int64_t prev1 = 1;
    int64_t current = 0;

    for (int i = 2; i <= index; ++i) {
        current = prev1 + prev2;
        prev2 = prev1;
        prev1 = current;
    }

    return current;
}

// ============================================================================
// FibonacciRecursionImpl - Recursive implementation
// ============================================================================

int64_t FibonacciRecursionImpl::calc(int index) const {
    // Validate input range
    if (index < 0 || index > 46) {
        throw std::out_of_range("Index must be between 0 and 46");
    }

    return calcRecursive(index);
}

int64_t FibonacciRecursionImpl::calcRecursive(int index) const {
    // Base cases
    if (index == 0) return 0;
    if (index == 1) return 1;

    // Recursive calculation
    return calcRecursive(index - 1) + calcRecursive(index - 2);
}

// ============================================================================
// Backward compatibility: free function wrapper
// ============================================================================

int64_t fibonacci(int index) {
    FibonacciLoopImpl impl;
    return impl.calc(index);
}
