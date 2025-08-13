# Web Build Instructions

This folder contains the HTML shell and JavaScript infrastructure for running the Infinite Runner game in a web browser using WebAssembly.

## Files

- **index.html** - Main HTML page with mock graphics demonstration
- **wasm-loader.js** - WebAssembly loading and interface utilities (template)
- **README.md** - This file

## Current Status

The HTML shell currently shows a **mock demonstration** of the game using Canvas 2D API. The actual WebAssembly module uses stub graphics functions and doesn't provide real visual output.

## Building for Web

1. **Build the WASM module:**
   ```bash
   cd ..
   zig build -Dtarget=wasm32-wasi -Dgraphics=raylib
   ```

2. **Copy the WASM file to web folder:**
   ```bash
   cp zig-out/bin/infinite-runner.wasm web/
   ```

3. **Serve the files (required due to CORS restrictions):**
   ```bash
   # Using Python 3
   cd web
   python3 -m http.server 8000
   
   # Or using Node.js with npx
   npx serve .
   
   # Or any other static file server
   ```

4. **Open in browser:**
   ```
   http://localhost:8000
   ```

## For Full WebAssembly Implementation

To make this a fully functional web game, you would need:

### 1. Graphics Backend
- Replace the stub implementations in `src/platform/web_stubs.h` with actual WebGL calls
- Implement proper Canvas/WebGL initialization and rendering
- Handle texture loading and management

### 2. WASM Integration
- Set up proper WASM imports/exports in `wasm-loader.js`
- Implement memory management between JS and WASM
- Create bindings for graphics operations

### 3. Raylib for Web
Alternative approach using Raylib's web support:
```bash
# Build Raylib for WebAssembly using Emscripten
emcc -o game.html main.c -I./raylib/include -L./raylib/lib -lraylib \
     -s USE_GLFW=3 -s ASYNCIFY -s TOTAL_MEMORY=67108864 \
     -s FORCE_FILESYSTEM=1 --shell-file shell.html
```

### 4. Input Handling
- Map browser keyboard/mouse events to game input
- Handle touch events for mobile support
- Implement gamepad support if needed

### 5. Audio
- Integrate Web Audio API for sound effects and music
- Handle audio context initialization (user interaction requirement)

## Demo Features

The current HTML demo includes:
- ✅ Canvas setup matching game dimensions (800x450)
- ✅ Mock rendering of test rectangles from main.c
- ✅ Basic game loop with start/pause/reset controls
- ✅ Keyboard input simulation (SPACE for jump)
- ✅ Animation showing obstacle movement
- ✅ Status display and game state management

## Development Notes

- The stub graphics functions allow the C code to compile for WebAssembly
- Real implementation would require significant additional work
- Consider using Emscripten + Raylib for faster web deployment
- Current build creates a WASM module but needs JS glue code for full functionality