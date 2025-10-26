#ifndef FIBONACCI_H
#define FIBONACCI_H

#include <cstdint>

/**
 * Fibonacci calculator.
 */
class Fibonacci {
public:
    /**
     * Calculate the Fibonacci number at the given index.
     *
     * @param index The index of the Fibonacci number to calculate
     * @return The Fibonacci number at the given index
     */
    int64_t calc(int index) const;
};

#endif // FIBONACCI_H
