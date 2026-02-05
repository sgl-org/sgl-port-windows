# SGL Simulator - Windows Build Guide

## Prerequisites

This project requires **64-bit MinGW-w64** to build, as the SDL2 libraries are compiled for 64-bit Windows.

### Option 1: Install MSYS2 (Recommended)

1. Download and install MSYS2 from: https://www.msys2.org/
2. Open "MSYS2 MSYS" from the Start menu
3. Update the package database:
   ```bash
   pacman -Syu
   ```
4. Install the toolchain:
   ```bash
   pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make
   ```
5. Add `C:\msys64\mingw64\bin` to your system PATH

### Option 2: Use the Build Script

If you have MSYS2 installed in the default location (`C:\msys64`), you can simply run:

```cmd
build.bat
```

The script will automatically set up the correct paths and build the project.

## Building the Project

### Using the build script (easiest):
```cmd
build.bat
```

### Using make directly:

1. Ensure MinGW-w64 bin directory is in your PATH
2. Navigate to the demo directory
3. Run:
   ```cmd
   make
   ```

## Running the Simulator

After a successful build, the executable will be in `build\sgl_simulator.exe`.

Run it with:
```cmd
build\sgl_simulator.exe
```

Or use the make target:
```cmd
make run
```

## Troubleshooting

### Error: "File format not recognized" or "undefined reference to SDL_*"

This means you're using 32-bit MinGW instead of 64-bit MinGW-w64. Make sure you have the 64-bit version installed and it's first in your PATH.

### Error: "gcc: command not found"

MinGW is not in your PATH. Either:
- Use `build.bat` which handles this automatically
- Add the MinGW-w64 bin directory to your system PATH
- Install MSYS2 and the required packages

## Clean Build

To remove all build artifacts:
```cmd
make clean
```

## Project Structure

- `main.c` - Main application entry point
- `sgl_port_sdl2.c` - SDL2 port implementation
- `test.c` - Test code
- `bg.c` - Background rendering
- `../sgl/` - SGL library source code
- `sdl/` - SDL2 library (headers, import libraries, and DLL)
- `build/` - Output directory for compiled objects and executable
