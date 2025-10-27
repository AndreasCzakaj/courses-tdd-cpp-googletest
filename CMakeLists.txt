cmake_minimum_required(VERSION 3.14)
project(FibonacciTDD)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch Google Test, nlohmann/json, and SQLite
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2
)
FetchContent_Declare(
  sqlite3
  URL https://www.sqlite.org/2024/sqlite-amalgamation-3450100.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest json)

# Make SQLite available and create library
FetchContent_GetProperties(sqlite3)
if(NOT sqlite3_POPULATED)
  FetchContent_Populate(sqlite3)
  add_library(sqlite3 ${sqlite3_SOURCE_DIR}/sqlite3.c)
  target_include_directories(sqlite3 PUBLIC ${sqlite3_SOURCE_DIR})
endif()

# Enable testing
enable_testing()

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Library
add_library(fibonacci_lib src/fibonacci.cpp)
add_library(login_service_lib src/login_service.cpp)
add_library(uss_lib src/uss.cpp)
target_link_libraries(uss_lib sqlite3)
target_link_libraries(login_service_lib uss_lib)

# Test executable
add_executable(fibonacci_tests tests/fibonacci_test.cpp)
add_executable(login_service_tests tests/login_service_test.cpp)
add_executable(matchers_tests tests/matchers_test.cpp)
add_executable(uss_tests tests/uss_test.cpp)
add_executable(repository_tests tests/repository_test.cpp)
add_executable(sqlite_repository_tests tests/sqlite_repository_test.cpp)

target_link_libraries(fibonacci_tests fibonacci_lib gtest_main gmock_main)
target_link_libraries(login_service_tests login_service_lib uss_lib gtest_main gmock_main)
target_link_libraries(matchers_tests gtest_main gmock_main nlohmann_json::nlohmann_json)
target_link_libraries(uss_tests uss_lib gtest_main gmock_main)
target_link_libraries(repository_tests uss_lib gtest_main gmock_main sqlite3)
target_link_libraries(sqlite_repository_tests uss_lib gtest_main gmock_main sqlite3)

# Register tests with CTest
include(GoogleTest)
gtest_discover_tests(fibonacci_tests)
gtest_discover_tests(login_service_tests)
gtest_discover_tests(matchers_tests)
gtest_discover_tests(uss_tests)
gtest_discover_tests(repository_tests)
gtest_discover_tests(sqlite_repository_tests)
