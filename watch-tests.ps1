# Continuous test runner for C++ TDD (PowerShell version for Windows)

$scriptPath = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $scriptPath

Write-Host "👀 Watching for file changes..." -ForegroundColor Cyan
Write-Host "Press Ctrl+C to stop" -ForegroundColor Yellow
Write-Host ""

# Install file watcher if not available
# You can install: choco install watchexec
# Or use built-in FileSystemWatcher

# Simple polling approach (works without additional tools)
$lastHash = ""

function Get-FilesHash {
    $files = Get-ChildItem -Recurse -Path "src","include","tests" -Include "*.cpp","*.h"
    $hashInput = ($files | Get-FileHash -Algorithm MD5 | Select-Object -ExpandProperty Hash) -join ""
    return $hashInput
}

function Run-BuildAndTests {
    Write-Host "🔨 Building..." -ForegroundColor Green

    Push-Location build

    # Build
    $buildOutput = cmake --build . 2>&1 | Select-Object -First 30
    $buildSuccess = $LASTEXITCODE -eq 0

    Write-Host $buildOutput

    if ($buildSuccess) {
        Write-Host ""
        Write-Host "🧪 Running tests..." -ForegroundColor Green
        Write-Host ""

        # Run all test executables
        Get-ChildItem -Filter "*_tests.exe" | ForEach-Object {
            Write-Host "Running $($_.Name)..." -ForegroundColor Cyan
            & $_.FullName --gtest_color=yes
            Write-Host ""
        }
    } else {
        Write-Host "❌ Build failed" -ForegroundColor Red
    }

    Pop-Location
}

# Initial run
Run-BuildAndTests
$lastHash = Get-FilesHash

# Watch loop
while ($true) {
    Start-Sleep -Seconds 1

    $currentHash = Get-FilesHash

    if ($currentHash -ne $lastHash) {
        Clear-Host
        Write-Host "🔄 File change detected..." -ForegroundColor Yellow
        Write-Host ""
        Run-BuildAndTests
        $lastHash = $currentHash
    }
}
