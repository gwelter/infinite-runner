#ifndef WEB_STUBS_H
#define WEB_STUBS_H

// Stub definitions for WebAssembly builds when libraries aren't available
// This allows the code to compile but won't have actual graphics functionality

#ifdef PLATFORM_WEB

// Raylib stubs
typedef struct {
    unsigned char r, g, b, a;
} Color;

typedef struct {
    float x, y, width, height;
} Rectangle;

// Function stubs - these would need to be implemented with WebGL/Canvas
static inline void InitWindow(int width, int height, const char* title) {
    (void)width; (void)height; (void)title;
    // In a real implementation, this would create a canvas element
}

static inline void CloseWindow(void) {
    // In a real implementation, this would cleanup the canvas
}

static inline int WindowShouldClose(void) {
    // In a real implementation, this would check for close events
    return 0; // Never close for now
}

static inline void SetTargetFPS(int fps) {
    (void)fps;
    // In a real implementation, this would set up requestAnimationFrame
}

static inline void BeginDrawing(void) {
    // In a real implementation, this would begin a WebGL/Canvas drawing context
}

static inline void EndDrawing(void) {
    // In a real implementation, this would present the frame
}

static inline void ClearBackground(Color color) {
    (void)color;
    // In a real implementation, this would clear the canvas
}

static inline void DrawRectangleRec(Rectangle rect, Color color) {
    (void)rect; (void)color;
    // In a real implementation, this would draw a rectangle using WebGL/Canvas
}

static inline void DrawText(const char* text, int x, int y, int size, Color color) {
    (void)text; (void)x; (void)y; (void)size; (void)color;
    // In a real implementation, this would draw text using Canvas 2D API
}

#endif // PLATFORM_WEB

#endif // WEB_STUBS_H