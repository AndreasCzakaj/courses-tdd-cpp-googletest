#!/bin/bash
# Continuous test runner for C++ TDD

cd "$(dirname "$0")"

echo "👀 Watching for file changes..."
echo "Press Ctrl+C to stop"
echo ""

# Watch source files and rebuild+test on change
#while true; do
    find src include tests -type f \( -name "*.cpp" -o -name "*.h" \) | \
    entr -d -c bash -c "
        echo '🔨 Building...'
        cd build && cmake --build . 2>&1 | head -30
        BUILD_STATUS=\${PIPESTATUS[0]}
        if [ \$BUILD_STATUS -eq 0 ]; then
            echo ''
            echo '🧪 Running tests...'
            echo ''

            test='uss_tests'

            echo \"Running \$test...\"
            ./\$test --gtest_color=yes
            echo ''
            
        else
            echo '❌ Build failed'
        fi
    "
#done
