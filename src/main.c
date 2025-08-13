#include "engine/graphics.h"
#ifndef NO_STDIO
#include <stdio.h>
#endif

int main(void) {
    // Initialize graphics with the backend selected at compile time
    #ifdef GRAPHICS_BACKEND_RAYLIB
        graphics_init(800, 450, "Infinite Runner - Raylib Backend", GRAPHICS_RAYLIB);
        #ifndef NO_STDIO
        printf("Running with Raylib backend\n");
        #endif
    #elif defined(GRAPHICS_BACKEND_SDL3)
        graphics_init(800, 450, "Infinite Runner - SDL3 Backend", GRAPHICS_SDL3);
        #ifndef NO_STDIO
        printf("Running with SDL3 backend\n");
        #endif
    #else
        #ifndef NO_STDIO
        printf("No graphics backend defined!\n");
        #endif
        return 1;
    #endif

    // Main game loop
    while (!graphics_should_close()) {
        graphics_begin_frame();
        
        // Clear screen with a dark blue color
        graphics_clear((GfxColor){20, 30, 80, 255});
        
        // Draw some test rectangles
        graphics_draw_rectangle((GfxRectangle){100, 100, 50, 50}, COLOR_RED);
        graphics_draw_rectangle((GfxRectangle){200, 150, 80, 30}, COLOR_GREEN);
        graphics_draw_rectangle((GfxRectangle){350, 200, 100, 100}, COLOR_BLUE);
        
        // Draw test text (Raylib only for now)
        #ifdef GRAPHICS_BACKEND_RAYLIB
            graphics_draw_text("Infinite Runner - Press ESC to close", 10, 10, 20, COLOR_WHITE);
            graphics_draw_text("WASD to test (placeholder)", 10, 40, 16, COLOR_GRAY);
        #endif
        
        graphics_end_frame();
    }

    // Cleanup
    graphics_shutdown();
    
    #ifndef NO_STDIO
    printf("Game closed successfully\n");
    #endif
    return 0;
}