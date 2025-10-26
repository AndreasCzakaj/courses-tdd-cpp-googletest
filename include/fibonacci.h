#ifndef FIBONACCI_H
#define FIBONACCI_H

#include <stdexcept>
#include <cstdint>
#include <memory>

/**
 * Interface for Fibonacci calculators.
 *
 * Valid indices are 0 to 46 (inclusive).
 * F(0) = 0, F(1) = 1, F(n) = F(n-1) + F(n-2)
 */
class IFibonacci {
public:
    virtual ~IFibonacci() = default;

    /**
     * Calculate the Fibonacci number at the given index.
     *
     * @param index The index of the Fibonacci number to calculate (0-46)
     * @return The Fibonacci number at the given index
     * @throws std::out_of_range if index is negative or greater than 46
     */
    virtual int64_t calc(int index) const = 0;
};

/**
 * Iterative (loop-based) implementation of Fibonacci calculator.
 */
class FibonacciLoopImpl : public IFibonacci {
public:
    int64_t calc(int index) const override;
};

/**
 * Recursive implementation of Fibonacci calculator.
 */
class FibonacciRecursionImpl : public IFibonacci {
public:
    int64_t calc(int index) const override;

private:
    int64_t calcRecursive(int index) const;
};

// Backward compatibility: free function that uses loop implementation
int64_t fibonacci(int index);

#endif // FIBONACCI_H
