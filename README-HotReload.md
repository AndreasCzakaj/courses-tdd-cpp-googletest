# Using Visual Studio Hot Reload with Tests

Visual Studio's Hot Reload feature doesn't work well with test executables because they start and finish quickly. Here are solutions:

## Solution 1: Use Visual Studio's Test Explorer (Recommended)

Visual Studio has built-in support for Google Test:

1. **Install Google Test Adapter** (if not already installed):
   - Extensions → Manage Extensions
   - Search for "Google Test Adapter"
   - Install it

2. **Open Test Explorer**:
   - Test → Test Explorer (or Ctrl+E, T)

3. **Build your project**:
   - The Test Explorer will automatically discover all your tests

4. **Enable "Run Tests After Build"**:
   - Test Explorer → Settings (gear icon)
   - Check "Run tests after build"

5. **Use Live Unit Testing** (Visual Studio Enterprise):
   - Test → Live Unit Testing → Start
   - Tests will run automatically as you type!

## Solution 2: Continuous Console Runner

For developers without Enterprise edition, use the PowerShell watch script:

```powershell
.\watch-tests.ps1
```

Or the batch file:
```cmd
watch-tests.bat
```

This keeps a process running that watches for file changes and reruns tests.

## Solution 3: CMake + Visual Studio Integration

1. **Open Folder** (not solution):
   - File → Open → Folder
   - Select the project root directory

2. **Configure CMake**:
   - Visual Studio will auto-detect CMakeLists.txt
   - It will configure the build automatically

3. **Use Test Explorer**:
   - Tests will appear in Test Explorer
   - Click "Run All" or enable "Run after build"

## Solution 4: Create a Long-Running Test Host

Build `test-runner-loop.cpp` which stays alive and reruns tests periodically. However, this has limitations with Hot Reload's rebuild requirements.

## Recommended Workflow for Visual Studio

**Best approach:**
1. Use Test Explorer with "Run tests after build" enabled
2. Make code changes
3. Build (Ctrl+Shift+B)
4. Tests automatically run in Test Explorer
5. See results instantly with green/red indicators

**For TDD workflow:**
- Keep Test Explorer open on a second monitor
- Write a failing test
- Watch it fail (red)
- Implement code
- Build
- Watch it pass (green)

## Hot Reload Limitations with Tests

Hot Reload in Visual Studio works best with:
- Applications that stay running (web apps, desktop apps)
- Debug sessions that remain active

It doesn't work well with:
- Unit tests (they exit immediately)
- Console apps that run and exit

**Workaround:** Use Test Explorer's continuous testing features instead!
