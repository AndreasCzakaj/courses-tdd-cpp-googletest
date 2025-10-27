@echo off
REM Batch file wrapper to run PowerShell watch script
REM This makes it easier to run on Windows - just double-click this file

powershell -ExecutionPolicy Bypass -File "%~dp0watch-tests.ps1"
