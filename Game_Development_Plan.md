# Infinite Runner Game Development Plan

## Overview
Complete development roadmap for a cross-platform infinite runner game built in C with interchangeable graphics backends (SDL3/Raylib) and WebAssembly support for web deployment.

**Core Concept:**
- Player character automatically runs left to right
- Dynamic obstacle generation requiring jump/crouch actions
- Progressive difficulty: speed increases every 5 seconds
- Cross-platform: Desktop (native) + Web (WebAssembly)

## Technology Architecture

### Core Technology Stack
```
Language:          C (C99/C11 standard)
Graphics:          Raylib (primary) / SDL3 (alternative)
Build System:      Zig Build System
WebAssembly:       Zig native WebAssembly support
Desktop Targets:   Windows, Linux, macOS
Web Target:        Browser (via WebAssembly)
```

### Project Structure
```
infinite-runner/
├── src/
│   ├── main.c                  # Entry point
│   ├── game/                   # Game logic
│   │   ├── game.h/.c          # Core game state
│   │   ├── player.h/.c        # Player entity
│   │   ├── obstacle.h/.c      # Obstacle system
│   │   ├── generator.h/.c     # Procedural generation
│   │   └── physics.h/.c       # Physics calculations
│   ├── engine/                # Engine abstraction
│   │   ├── graphics.h/.c      # Graphics API
│   │   ├── input.h/.c         # Input handling
│   │   ├── audio.h/.c         # Audio system
│   │   └── math.h/.c          # Math utilities
│   └── platform/              # Backend implementations
│       ├── raylib_impl.c      # Raylib backend
│       └── sdl3_impl.c        # SDL3 backend
├── assets/                    # Game assets
├── zig-out/                   # Build output
├── web/                       # WebAssembly files
└── build.zig                  # Zig build configuration
```

## Graphics Abstraction System

### Unified Graphics API
```c
// Core graphics interface
typedef enum {
    GRAPHICS_RAYLIB,
    GRAPHICS_SDL3
} GraphicsBackend;

typedef struct {
    float x, y, width, height;
} Rectangle;

typedef struct {
    unsigned char r, g, b, a;
} Color;

// Essential functions
void graphics_init(int width, int height, const char* title, GraphicsBackend backend);
void graphics_shutdown(void);
bool graphics_should_close(void);
void graphics_begin_frame(void);
void graphics_end_frame(void);
void graphics_clear(Color color);
void graphics_draw_rectangle(Rectangle rect, Color color);
void graphics_draw_texture(int texture_id, Rectangle dest, Color tint);
void graphics_draw_text(const char* text, int x, int y, int size, Color color);
int graphics_load_texture(const char* filename);
void graphics_unload_texture(int texture_id);
```

### Backend Implementation Strategy
- **Raylib Backend**: Direct function mapping (simpler implementation)
- **SDL3 Backend**: Manual texture/rendering management
- **Compile-time Selection**: Preprocessor macros for backend choice
- **Interchangeable**: Same game code works with both backends

## Build System Configuration

### Zig Build Configuration
```zig
// build.zig
const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    // Build options for graphics backend selection
    const graphics_backend = b.option(
        enum { raylib, sdl3 },
        "graphics",
        "Graphics backend to use (raylib or sdl3)"
    ) orelse .raylib;

    const exe = b.addExecutable(.{
        .name = "infinite-runner",
        .root_source_file = b.path("src/main.c"),
        .target = target,
        .optimize = optimize,
    });

    // Add C source files
    exe.addCSourceFiles(.{
        .files = &.{
            "src/game/game.c",
            "src/game/player.c",
            "src/game/obstacle.c",
            "src/game/generator.c",
            "src/game/physics.c",
            "src/engine/graphics.c",
            "src/engine/input.c",
            "src/engine/audio.c",
            "src/engine/math.c",
        },
        .flags = &.{"-std=c99", "-Wall", "-Wextra"},
    });

    // Add platform-specific backend
    switch (graphics_backend) {
        .raylib => {
            exe.addCSourceFile(.{ .file = b.path("src/platform/raylib_impl.c") });
            exe.defineCMacro("GRAPHICS_BACKEND_RAYLIB", "1");
            
            // Link Raylib (system or package manager)
            exe.linkSystemLibrary("raylib");
            exe.linkLibC();
        },
        .sdl3 => {
            exe.addCSourceFile(.{ .file = b.path("src/platform/sdl3_impl.c") });
            exe.defineCMacro("GRAPHICS_BACKEND_SDL3", "1");
            
            // Link SDL3
            exe.linkSystemLibrary("SDL3");
            exe.linkLibC();
        },
    }

    // WebAssembly target configuration
    if (target.result.cpu.arch == .wasm32) {
        exe.defineCMacro("PLATFORM_WEB", "1");
        // Assets will be embedded or loaded asynchronously
    }

    b.installArtifact(exe);

    // Run command
    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step("run", "Run the game");
    run_step.dependOn(&run_cmd.step);
}
```

### Development Commands
```bash
# Desktop builds with different backends
zig build -Dgraphics=raylib              # Build with Raylib
zig build -Dgraphics=sdl3                # Build with SDL3

# WebAssembly builds
zig build -Dtarget=wasm32-freestanding -Dgraphics=raylib  # Web + Raylib
zig build -Dtarget=wasm32-freestanding -Dgraphics=sdl3   # Web + SDL3

# Run desktop build
zig build run -Dgraphics=raylib

# Development builds (debug)
zig build -Doptimize=Debug -Dgraphics=raylib

# Release builds
zig build -Doptimize=ReleaseFast -Dgraphics=raylib

# Clean build artifacts
rm -rf zig-out zig-cache
```

## Development Phases

### Phase 1: Foundation & Build System
**Task 1.1: Development Environment Setup**
- [x] Install Zig compiler (latest stable version) and verify with `zig version`
- [x] Install GCC/Clang compiler for C compilation support
- [x] Install Git for version control
- [x] Set up preferred text editor with C and Zig language support

**Task 1.2: Raylib Installation & Verification**
- [x] Download and install Raylib from official repository
- [x] Create test program: `InitWindow(800, 450, "Test"); while(!WindowShouldClose()) { BeginDrawing(); ClearBackground(RAYWHITE); EndDrawing(); } CloseWindow();`
- [x] Verify successful compilation and window creation
- [x] Test basic drawing functions (DrawRectangle, DrawText)

**Task 1.3: SDL3 Installation & Verification**
- [x] Download and install SDL3 development libraries
- [x] Create test program with SDL_Init, SDL_CreateWindow, SDL_CreateRenderer
- [x] Verify successful compilation and window creation
- [x] Test basic rendering (SDL_SetRenderDrawColor, SDL_RenderFillRect)

**Task 1.4: WebAssembly Setup with Zig**
- [x] Verify Zig's WebAssembly target support with `zig targets`
- [x] Create simple test program and compile to WebAssembly
- [x] Test basic compilation: `zig build-exe hello.c -target wasm32-freestanding`
- [x] Set up basic HTML wrapper for WebAssembly execution

**Task 1.5: Project Structure Creation**
- [x] Create directory structure as defined in plan
- [x] Initialize Git repository with .gitignore for build artifacts
- [x] Create placeholder .c/.h files with basic headers
- [x] Add README.md with build instructions

**Task 1.6: Basic Zig Build Configuration**
- [x] Create build.zig with basic executable target
- [x] Configure C99 standard and compiler flags (-Wall, -Wextra)
- [x] Add graphics backend selection option (raylib/sdl3)
- [x] Test build with `zig build -Dgraphics=raylib`

**MILESTONE:** Project compiles successfully with Zig, both Raylib and SDL3 backends accessible, WebAssembly builds work.

### Phase 1 Continued: Graphics Abstraction
**Task 1.7: Graphics API Design**
- [x] Create `src/engine/graphics.h` with unified interface
- [x] Define core data structures (Rectangle, Color, Texture2D)
- [x] Define function signatures for window, drawing, and resource management
- [x] Add GraphicsBackend enum and initialization function
- [x] Include proper header guards and C99 compatibility

**Task 1.8: Raylib Backend Implementation**
- [x] Create `src/platform/raylib_impl.c` with all graphics.h functions
- [x] Map Raylib functions directly (InitWindow → graphics_init)
- [x] Handle Raylib Color/Rectangle type conversions
- [ ] Implement texture loading using LoadTexture/UnloadTexture
- [x] Test with simple rectangle and text rendering

**Task 1.9: Backend Selection Build System**
- [x] Extend build.zig with graphics backend option handling
- [x] Add conditional compilation with preprocessor directives
- [x] Configure library linking based on backend choice
- [x] Test backend switching: `zig build -Dgraphics=raylib` and `zig build -Dgraphics=sdl3`
- [x] Verify both backends compile and link correctly

**Task 1.10: Core Game Loop Structure**
- [x] Create `src/game/game.h` with GameState enum (MENU, PLAYING, GAME_OVER)
- [x] Implement basic game loop in main.c using graphics abstraction
- [ ] Add delta time calculation for frame-rate independent movement
- [ ] Create state transition functions (game_init, game_update, game_render)
- [ ] Implement clean shutdown and resource cleanup

**Task 1.11: Input Abstraction Layer**
- [ ] Create `src/engine/input.h` with unified input interface
- [ ] Define InputState structure with keyboard and mouse states
- [ ] Implement input polling for Raylib (IsKeyPressed, IsKeyDown)
- [ ] Add input update function called each frame
- [ ] Test with simple keyboard input (SPACE, arrows) display on screen

**MILESTONE:** Graphics abstraction compiles with Raylib, basic window renders colored rectangles, input system responds to keyboard, clean game loop structure established.

### Phase 2: Player Entity and Game Mechanics
**Task 2.1: Player Entity Implementation**
- [ ] Create `src/game/player.h` with Player struct (position, velocity, size, state)
- [ ] Define PlayerState enum (RUNNING, JUMPING, CROUCHING, DEAD)
- [ ] Implement player_create, player_update, player_render functions
- [ ] Add player movement with constant horizontal velocity (200px/s)
- [ ] Test player rectangle rendering and horizontal movement

**Task 2.2: Physics System Foundation**
- [ ] Create `src/game/physics.h` with physics constants (GRAVITY = 800px/s²)
- [ ] Implement basic 2D vector operations (add, multiply, normalize)
- [ ] Add physics_update function for position integration
- [ ] Create collision detection functions (rectangle overlap)
- [ ] Test gravity application and ground collision

**Task 2.3: Jump Mechanics Implementation**
- [ ] Add jump input detection (SPACE key pressed)
- [ ] Implement jump velocity (-350px/s upward)
- [ ] Add ground detection and landing logic
- [ ] Prevent multiple jumps (only when grounded)
- [ ] Fine-tune jump feel with proper gravity and timing

**Task 2.4: Crouch Mechanics Implementation**
- [ ] Add crouch input detection (DOWN arrow held)
- [ ] Modify player hitbox height when crouching
- [ ] Implement crouch animation state transitions
- [ ] Add visual feedback for crouching state
- [ ] Test crouch/uncrouch responsiveness

**Task 2.5: Basic Obstacle System**
- [ ] Create `src/game/obstacle.h` with Obstacle struct (position, size, type)
- [ ] Define ObstacleType enum (HIGH, LOW, GAP)
- [ ] Implement obstacle_create, obstacle_update, obstacle_render
- [ ] Add simple obstacle spawning at fixed intervals
- [ ] Test obstacle movement and removal when off-screen

**Task 2.6: Collision Detection System**
- [ ] Implement player-obstacle collision using AABB (Axis-Aligned Bounding Box)
- [ ] Add collision response (trigger game over state)
- [ ] Create forgiving hitbox (smaller than visual sprite)
- [ ] Test collision accuracy with different obstacle types
- [ ] Add visual feedback for collisions (screen shake, color change)

**MILESTONE:** Player can run, jump, crouch with proper physics. Obstacles spawn and move. Collision detection triggers game over. All mechanics feel responsive at 60fps.

### Phase 3: Procedural Generation and Multi-Platform
**Task 3.1: Infinite Scrolling Background**
- [ ] Create `src/game/generator.h` with background generation system
- [ ] Implement parallax scrolling with 3 layers (0.2x, 0.5x, 0.8x speeds)
- [ ] Add tile-based ground platform with seamless repetition
- [ ] Create background element generation (clouds, distant objects)
- [ ] Test smooth scrolling at different game speeds

**Task 3.2: Obstacle Generation Algorithm**
- [ ] Implement weighted random obstacle type selection
- [ ] Create spawn timing based on current game speed (4s → 2.5s spacing)
- [ ] Add minimum safe distance calculations between obstacles
- [ ] Implement obstacle cleanup when far behind player
- [ ] Test consistent obstacle flow without gaps or overlaps

**Task 3.3: Difficulty Progression System**
- [ ] Create difficulty scaling every 5 seconds (15% speed increase)
- [ ] Implement progressive obstacle frequency reduction
- [ ] Add pattern complexity scaling (single → pairs → sequences)
- [ ] Create visual feedback for difficulty increases
- [ ] Test balanced progression curve and maximum speed cap

**Task 3.4: SDL3 Backend Implementation**
- [ ] Create `src/platform/sdl3_impl.c` implementing graphics.h interface
- [ ] Map SDL3 functions (SDL_CreateWindow, SDL_CreateRenderer, etc.)
- [ ] Handle SDL3 texture management and rendering pipeline
- [ ] Implement SDL3 event handling for input abstraction
- [ ] Test complete backend switching compilation

**Task 3.5: WebAssembly Build Configuration**
- [ ] Extend build.zig with WebAssembly target configuration
- [ ] Configure web-specific settings and memory allocation
- [ ] Add web-specific input handling (touch events, pointer lock)
- [ ] Create HTML shell template for proper canvas scaling
- [ ] Test WebAssembly compilation: `zig build -Dtarget=wasm32-freestanding -Dgraphics=raylib`

**Task 3.6: Cross-Platform Input Handling**
- [ ] Extend input system for touch/mouse events (web compatibility)
- [ ] Add input mapping for mobile devices (tap=jump, swipe=crouch)
- [ ] Implement pointer lock for desktop mouse control
- [ ] Add responsive input areas for touch interfaces
- [ ] Test input consistency across desktop and web platforms

**MILESTONE:** Both graphics backends work interchangeably. WebAssembly build runs in browser. Procedural generation creates infinite, challenging gameplay. Touch controls work on mobile devices.

### Phase 4: Audio, Polish, and Final Integration
**Task 4.1: Audio Abstraction Layer**
- [ ] Create `src/engine/audio.h` with unified audio interface
- [ ] Define Sound and Music structs with resource management
- [ ] Implement audio_init, audio_play_sound, audio_play_music functions
- [ ] Add volume controls and audio state management
- [ ] Create Raylib audio backend using InitAudioDevice, LoadSound

**Task 4.2: SDL3 Audio Backend**
- [ ] Implement SDL3 audio backend in `src/platform/sdl3_impl.c`
- [ ] Use SDL_OpenAudioDevice and SDL_AudioSpec for audio setup
- [ ] Handle audio callback functions and mixing
- [ ] Implement WAV file loading and playback
- [ ] Test audio backend switching between Raylib and SDL3

**Task 4.3: Game Audio Integration**
- [ ] Add sound effects for jump, collision, obstacle pass events
- [ ] Implement background music with loop functionality
- [ ] Create audio feedback for speed increases and achievements
- [ ] Add web-compatible audio handling (user interaction requirement)
- [ ] Test audio timing and synchronization with gameplay

**Task 4.4: UI and Game States**
- [ ] Implement menu system with start/restart functionality
- [ ] Add game over screen with score display and restart button
- [ ] Create HUD elements (score, speed, time survived)
- [ ] Implement pause functionality and state management
- [ ] Add visual feedback systems (screen shake, particle effects)

**Task 4.5: Performance Optimization**
- [ ] Profile memory usage and optimize for WebAssembly constraints
- [ ] Implement object pooling for obstacles and particles
- [ ] Optimize rendering calls and texture management
- [ ] Add frame rate monitoring and performance metrics
- [ ] Test 60fps stability across all platforms and difficulty levels

**Task 4.6: Asset Pipeline and Deployment**
- [ ] Create asset loading system with fallback placeholder graphics
- [ ] Optimize asset sizes for web deployment
- [ ] Implement build scripts for automated compilation
- [ ] Create deployment packages for desktop and web platforms
- [ ] Test final builds on target platforms and browsers

**Task 4.7: Cross-Platform Testing and Bug Fixes**
- [ ] Test complete game flow on Windows, Linux, macOS
- [ ] Verify WebAssembly functionality in multiple browsers
- [ ] Test touch controls on mobile devices via browser
- [ ] Fix platform-specific issues and edge cases
- [ ] Validate memory management and resource cleanup

**MILESTONE:** Complete game with audio runs on desktop (both backends) and web. Performance maintains 60fps. All platforms provide consistent gameplay experience. Game is deployable and distributable.

## Core Game Mechanics

### Player System
- **Movement**: Constant horizontal velocity (200px/s baseline)
- **Controls**: 
  - Jump: Spacebar/Up Arrow/Screen Tap (-350px/s initial velocity)
  - Crouch: Down Arrow/Swipe Down (reduce hitbox height)
- **Physics**: Gravity at 800px/s², responsive feel
- **Collision**: Forgiving hitbox (smaller than visual sprite)

### Obstacle Types
1. **High Obstacles**: Require jumping (boxes, barriers)
2. **Low Obstacles**: Require crouching (overhead hazards)
3. **Ground Gaps**: Require precise jump timing
4. **Pattern Combinations**: Mixed obstacles at higher difficulties

### Difficulty Progression
```
Timeline:  0-5s    5-10s   10-15s   15s+
Speed:     +0%     +15%    +30%     +45% (capped at 2.5x)
Spacing:   4.0s    3.5s    3.0s     2.5s minimum
Patterns:  Single  Mixed   Pairs    Complex sequences
```

## Technical Implementation Details

### Memory Management Strategy
- **Manual Management**: Explicit malloc/free for dynamic content
- **Asset Loading**: Efficient texture/sound management
- **Web Optimization**: Minimize memory footprint for WebAssembly

### Cross-Platform Considerations
- **Input Handling**: Keyboard (desktop) + touch (web)
- **Asset Loading**: Synchronous (desktop) vs preloaded (web)
- **Performance**: Native optimization vs WebAssembly constraints
- **File Paths**: Consistent relative path handling

## Development Workflow

### Primary Development Flow
```
Desktop Development (Raylib) → SDL3 Testing → Web Build Validation
        ↑                           ↓
    Fast iteration              Compatibility check
```

### Build Targets
1. **Desktop + Raylib**: Primary development (fastest iteration)
2. **Desktop + SDL3**: Compatibility validation
3. **Web + Raylib**: WebAssembly deployment testing
4. **Web + SDL3**: Alternative web deployment option

## Required Development Tools
- **Zig Compiler** (latest stable) - Build system and compilation
- **C Compiler** (GCC/Clang) - C code compilation support
- **Raylib** - Primary graphics library
- **SDL3** - Alternative graphics library
- **Text Editor** with C and Zig language support

## Key Technical Advantages
1. **Performance**: Native C performance on desktop
2. **Portability**: Single codebase for desktop and web
3. **Flexibility**: Choice between graphics backends
4. **Learning**: Understanding low-level game development
5. **Deployment**: Web distribution via WebAssembly

## Notes
- Plan created with focus on interchangeable graphics backends
- WebAssembly support enables web deployment without code changes
- Modular architecture allows for easy feature expansion
- Progressive difficulty system ensures engaging gameplay