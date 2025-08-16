# Infinite Runner

A cross-platform infinite runner game built in C with interchangeable graphics backends (Raylib/SDL3) using Zig build system.

## Prerequisites

- Zig compiler (latest stable version)
- Raylib development libraries
- SDL3 development libraries

### Installing Dependencies

**Ubuntu/Debian:**
```bash
# Install Raylib
sudo apt install libraylib-dev

# SDL3 is not yet available in most package managers
# For now, use Raylib backend or build SDL3 from source
# Check SDL3 releases: https://github.com/libsdl-org/SDL/releases
```

**macOS (with Homebrew):**
```bash
brew install raylib
# SDL3 may need to be built from source
```

**Windows:**
- Download Raylib from: https://github.com/raysan5/raylib/releases
- Download SDL3 from: https://github.com/libsdl-org/SDL/releases

## Building

### Build with Raylib
```bash
zig build -Dgraphics=raylib
```

### Build with SDL3 (Default)
```bash
zig build -Dgraphics=sdl3
```

### Run the game
```bash
# With Raylib
zig build run -Dgraphics=raylib

# With SDL3
zig build run -Dgraphics=sdl3
```

### Debug builds
```bash
zig build -Doptimize=Debug -Dgraphics=raylib
```

### Release builds
```bash
zig build -Doptimize=ReleaseFast -Dgraphics=raylib
```

### WebAssembly builds
```bash
# Build WASM module (SDL3 for now) and copy to web folder
zig build wasm

# Serve the web files
cd web
python3 -m http.server 8000

# Open browser to http://localhost:8000
```

**Note:** WebAssembly builds currently use stub implementations for graphics functions. The HTML shell provides a mock demonstration of the game. For full web functionality, you would need Raylib compiled for WebAssembly or a WebGL-based implementation.

## Project Structure

```
infinite-runner/
├── src/
│   ├── main.c                  # Entry point
│   ├── engine/                 # Engine abstraction
│   │   └── graphics.h/.c       # Graphics API
│   └── platform/               # Backend implementations
│       ├── raylib_impl.c       # Raylib backend
│       └── sdl3_impl.c         # SDL3 backend
├── web/                        # WebAssembly web shell
│   ├── index.html              # HTML page with demo
│   ├── game.js          # WASM loader utilities
├── assets/                     # Game assets
└── build.zig                   # Zig build configuration
```

## Controls

- ESC: Close window (Raylib)
- Close button: Close window (both backends)

## Current Status

This is a basic boilerplate with:
- ✅ Graphics abstraction layer
- ✅ Raylib backend with window and basic drawing (tested and working)
- ⚠️ SDL3 backend with window and basic drawing (compiles but requires SDL3 installation)
- ✅ Interchangeable backends at compile time
- ✅ WebAssembly compilation support (for SDL3)
- 🚧 Game mechanics (player, obstacles, physics) - TODO
- 🚧 Audio system - TODO
- 🚧 Input handling - TODO

**Notes:**
- SDL3 is still in development and not widely available in package managers
- WebAssembly builds use graphics stubs; full web support requires Raylib WASM build
- Project compiles successfully for native and WebAssembly (with SDL3 library), and Raylib