# Fibonacci TDD Example with Google Test

This project demonstrates Test-Driven Development (TDD) in C++ using Google Test framework.

## Project Structure

```
tdd-c++/
├── CMakeLists.txt          # Build configuration
├── include/
│   └── fibonacci.h         # Fibonacci function declaration
├── src/
│   └── fibonacci.cpp       # Fibonacci implementation
└── tests/
    └── fibonacci_test.cpp  # Google Test test cases
```

## Requirements

- CMake 3.14 or higher
- C++17 compatible compiler (GCC, Clang, or MSVC)
- Internet connection (first build only, to fetch Google Test)

## Fibonacci Function Specification

The `fibonacci(int index)` function:
- Accepts indices from 0 to 46 (inclusive)
- Returns the Fibonacci number at the given index
- F(0) = 0, F(1) = 1, F(n) = F(n-1) + F(n-2)
- Throws `std::out_of_range` exception for:
  - Negative indices (< 0)
  - Indices greater than 46 (> 46)

**Note:** The limit of 46 is because F(47) = 2,971,215,073 exceeds the range of 32-bit signed integers. We use `int64_t` for return values to safely handle all values up to F(46).

## Test Coverage

The test suite includes:

### Valid Input Tests
- Base cases (indices 0 and 1)
- Small indices (2-10)
- Medium indices (15, 20, 25, 30)
- Maximum valid indices (45, 46)
- Parameterized tests for known values

### Error Handling Tests
- Negative indices (-1, -2, -10, -100, INT_MIN)
- Indices too large (47, 48, 50, 100, 1000, INT_MAX)
- Exception message verification
- Parameterized tests for all invalid ranges

## Building the Project

```bash
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build the project
cmake --build .
```

## Running the Tests

```bash
# From the build directory
./fibonacci_tests

# Or using CTest
ctest --output-on-failure
```

## Expected Test Output

All tests should pass:
```
[==========] Running 38 tests from 5 test suites.
[----------] Global test environment set-up.
...
[==========] 38 tests from 5 test suites ran.
[  PASSED  ] 38 tests.
```

## TDD Workflow Example

This project follows the TDD cycle:

1. **Red**: Write a failing test
2. **Green**: Write minimal code to make the test pass
3. **Refactor**: Improve code while keeping tests green

Example workflow:
```cpp
// 1. Write test first (tests/fibonacci_test.cpp)
TEST_F(FibonacciTest, ReturnsZeroForIndexZero) {
    EXPECT_EQ(fibonacci(0), 0);
}

// 2. Run test (it fails - function not implemented)
// 3. Implement minimal code (src/fibonacci.cpp)
int64_t fibonacci(int index) {
    if (index == 0) return 0;
    // ...
}

// 4. Run test again (it passes)
// 5. Refactor if needed
// 6. Repeat for next test case
```

## Key Learning Points

1. **Test Organization**: Tests are organized by functionality (base cases, valid inputs, error cases)
2. **Parameterized Tests**: Reduce code duplication for similar test scenarios
3. **Exception Testing**: Verify both that exceptions are thrown and their messages
4. **Boundary Testing**: Test edge cases at the limits of valid ranges
5. **Test Fixtures**: Use `TEST_F` for shared setup/teardown code

## Extending the Project

To practice TDD, try:
- Adding memoization to improve performance
- Supporting larger indices using `uint64_t` or big integer libraries
- Adding a function to generate Fibonacci sequences
- Implementing recursive vs. iterative benchmarks
