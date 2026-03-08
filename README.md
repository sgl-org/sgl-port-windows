# SGL Port for Windows

A Windows SDL2-based port and demo for the Simple Graphics Library (SGL).

## Overview

This repository provides a Windows implementation of SGL using SDL2 as the graphics backend. It includes a complete demo application showcasing various SGL features including widgets, graphics primitives, and user interface elements.

## Prerequisites

This project requires **64-bit MinGW-w64** (not 32-bit MinGW) because the included SDL2 libraries are compiled for 64-bit Windows.

### Recommended: MSYS2 with MinGW-w64

1. Download and install [MSYS2](https://www.msys2.org/)
2. Open "MSYS2 MSYS" from the Start menu
3. Update the package database:
   ```bash
   pacman -Syu
   ```
4. Install the required toolchain:
   ```bash
   pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make
   ```
5. Add `C:\msys64\mingw64\bin` to your system PATH environment variable

## Quick Start

### Clone the Repository

```bash
git clone https://github.com/sgl-org/sgl-port-windows.git
cd sgl-port-windows
git submodule init
git submodule update --remote
```

### Build the Demo

#### Option 1: Using the build script (easiest)
1. Enter demo directory
2. run build.bat script

The script automatically detects and sets up the correct MinGW-w64 toolchain, and run the exe app

#### Option 2: Using make directly

Ensure MinGW-w64 is in your PATH, then:
```bash
cd demo
make -j8
or
mingw32-make -j8
```

### Run the Demo

```bash
make run
or
mingw32-make run
```

Or directly:
```cmd
build\sgl_simulator.exe
```

## Project Structure

```
sgl-port-windows/
├── demo/                      # Demo application
│   ├── main.c                # Main application entry point
│   ├── sgl_port_sdl2.c       # SDL2 port implementation
│   ├── test.c                # Test/demo code
│   ├── bg.c                  # Background rendering
│   ├── sdl/                  # SDL2 library (headers, libs, DLLs)
│   ├── Makefile              # Build configuration
│   ├── build.bat             # Windows build script
│   └── BUILD.md              # Detailed build instructions
└── sgl/                       # SGL library (git submodule)
    └── source/               # SGL source code
```

## Features

The demo showcases:
- Various SGL widgets (buttons, sliders, checkboxes, text input, etc.)
- Graphics primitives (lines, rectangles, circles, arcs, rings)
- Text rendering with multiple fonts
- Event handling (mouse and keyboard input)
- Memory management with lwmem
- Animation support

## Building from Scratch

### Clean Build

```bash
cd demo
make clean
make -j8
```

### Build Targets

- `make` or `make all` - Build the executable
- `make clean` - Remove all build artifacts
- `make run` - Build and run the application

## Troubleshooting

### Error: "File format not recognized" or "undefined reference to SDL_*"

You're using 32-bit MinGW instead of 64-bit MinGW-w64. The SDL2 libraries in this project are built for x86_64 (64-bit). Install MSYS2 with MinGW-w64 as described in the Prerequisites section.

### Error: "gcc: command not found"

MinGW-w64 is not in your PATH. Either:
- Use `build.bat` which handles PATH setup automatically
- Add `C:\msys64\mingw64\bin` to your system PATH
- Run from MSYS2 MinGW 64-bit shell

### Application won't start or SDL2.dll missing

The SDL2.dll should be automatically copied to the build directory when you run `make` or `make run`. If it's missing, manually copy it:
```cmd
copy sdl\bin\SDL2.dll build\
```

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

## License

See the LICENSE file in the SGL submodule for library licensing information.

## Related Projects

- [SGL (Simple Graphics Library)](https://github.com/sgl-org/sgl) - The main SGL library
- [SDL2](https://www.libsdl.org/) - Simple DirectMedia Layer

## Additional Documentation

For more detailed build instructions, troubleshooting, and configuration options, see [demo/BUILD.md](demo/BUILD.md).
