@echo off
REM Build script for SGL Simulator on Windows
REM This script sets up the correct toolchain path and builds the project

echo Setting up build environment...

REM Check if 64-bit MinGW is available
set MINGW64_PATH=C:\msys64\mingw64\bin
set MINGW_ALT_PATH=C:\mingw64\bin
set MAKE_CMD=make

if exist "%MINGW64_PATH%\gcc.exe" (
    set "PATH=%MINGW64_PATH%;%PATH%"
    echo Using MinGW64 from %MINGW64_PATH%
) else if exist "%MINGW_ALT_PATH%\gcc.exe" (
    set "PATH=%MINGW_ALT_PATH%;%PATH%"
    echo Using MinGW64 from %MINGW_ALT_PATH%
) else (
    echo ERROR: 64-bit MinGW-w64 not found!
    echo.
    echo Please install MSYS2 with MinGW-w64 from: https://www.msys2.org/
    echo After installation, run: pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make
    echo.
    echo Or ensure MinGW-w64 is installed and update the paths in this script.
    pause
    exit /b 1
)

REM Check for mingw32-make first, then fallback to make
if exist "%MINGW64_PATH%\mingw32-make.exe" (
    set MAKE_CMD=mingw32-make
    echo Using mingw32-make
) else (
    set MAKE_CMD=make
    echo Using make
)

echo.
echo Building project...
%MAKE_CMD%

if %ERRORLEVEL% == 0 (
    echo.
    echo Build successful! The executable is in build\sgl_simulator.exe
    echo To run it, execute: build\sgl_simulator.exe
) else (
    echo.
    echo Build failed! Check the error messages above.
)

pause
