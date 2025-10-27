#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>

// Continuous test runner that stays alive
// This allows Visual Studio's Hot Reload to work with tests
// Build this as a separate executable that runs tests in a loop

int main(int argc, char** argv) {
    std::cout << "🔄 Continuous Test Runner for Visual Studio Hot Reload\n";
    std::cout << "Press Ctrl+C to stop\n";
    std::cout << "Waiting for code changes...\n\n";

    int iteration = 1;

    while (true) {
        std::cout << "\n=== Test Run #" << iteration << " ===\n";

        // Run all test executables
        // Note: Adjust paths based on your build configuration
        std::system("cd build && fibonacci_tests.exe --gtest_color=yes");
        std::system("cd build && uss_tests.exe --gtest_color=yes");
        std::system("cd build && repository_tests.exe --gtest_color=yes");
        std::system("cd build && sqlite_repository_tests.exe --gtest_color=yes");
        std::system("cd build && login_service_tests.exe --gtest_color=yes");

        std::cout << "\n✅ Tests completed. Waiting for next Hot Reload...\n";
        std::cout << "   (Hot Reload will automatically re-run tests)\n";

        // Wait before next iteration
        // Hot Reload should trigger a rebuild and this process stays alive
        std::this_thread::sleep_for(std::chrono::seconds(2));

        iteration++;
    }

    return 0;
}
