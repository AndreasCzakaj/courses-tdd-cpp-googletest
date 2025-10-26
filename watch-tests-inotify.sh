#!/bin/bash
# Continuous test runner using inotifywait

cd "$(dirname "$0")"

echo "👀 Watching for file changes..."
echo "Press Ctrl+C to stop"
echo ""

run_tests() {
    clear
    echo "🔨 Building..."
    cd build && cmake --build . 2>&1
    if [ $? -eq 0 ]; then
        echo ""
        echo "🧪 Running tests..."
        echo ""
        ./fibonacci_tests --gtest_color=yes
        echo ""
        echo "⏰ $(date '+%H:%M:%S') - Waiting for changes..."
    else
        echo "❌ Build failed - fix errors and save to retry"
    fi
}

# Run once initially
run_tests

# Watch for changes
while inotifywait -r -e modify,create,delete src include tests 2>/dev/null; do
    sleep 0.5  # Debounce multiple rapid changes
    run_tests
done
