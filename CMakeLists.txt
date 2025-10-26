cmake_minimum_required(VERSION 3.14)
project(FibonacciTDD)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Library
add_library(fibonacci_lib src/fibonacci.cpp)

# Test executable
add_executable(fibonacci_tests tests/fibonacci_test.cpp)
target_link_libraries(fibonacci_tests fibonacci_lib gtest_main)

# Register tests with CTest
include(GoogleTest)
gtest_discover_tests(fibonacci_tests)
