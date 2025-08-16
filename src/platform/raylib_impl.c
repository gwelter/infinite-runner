#ifdef GRAPHICS_BACKEND_RAYLIB

#include "../engine/graphics.h"
#include <raylib.h>

// Convert our GfxColor to Raylib Color
static Color raylib_color_from_gfx_color(GfxColor color) {
    return (Color){color.r, color.g, color.b, color.a};
}

// Convert our GfxRectangle to Raylib Rectangle
static Rectangle raylib_rectangle_from_gfx_rectangle(GfxRectangle rect) {
    return (Rectangle){rect.x, rect.y, rect.width, rect.height};
}

void platform_graphics_init(int width, int height, const char* title) {
    InitWindow(width, height, title);
    SetTargetFPS(60);
}

void platform_graphics_shutdown(void) {
    CloseWindow();
}

bool platform_graphics_should_close(void) {
    return WindowShouldClose();
}

void platform_graphics_begin_frame(void) {
    BeginDrawing();
}

void platform_graphics_end_frame(void) {
    EndDrawing();
}

void platform_graphics_clear(GfxColor color) {
    ClearBackground(raylib_color_from_gfx_color(color));
}

void platform_graphics_draw_rectangle(GfxRectangle rect, GfxColor color) {
    Rectangle raylib_rect = raylib_rectangle_from_gfx_rectangle(rect);
    Color raylib_color = raylib_color_from_gfx_color(color);
    DrawRectangleRec(raylib_rect, raylib_color);
}

void platform_graphics_draw_texture(int texture_id, GfxRectangle dest, GfxColor tint) {
    // For now, we'll implement this as a placeholder
    // In a full implementation, we'd maintain a texture registry
    (void)texture_id;
    (void)dest;
    (void)tint;
}

void platform_graphics_draw_text(const char* text, int x, int y, int size, GfxColor color) {
    Color raylib_color = raylib_color_from_gfx_color(color);
    DrawText(text, x, y, size, raylib_color);
}

int platform_graphics_load_texture(const char* filename) {
    // For now, return a dummy texture ID
    // In a full implementation, we'd load the texture and return a handle
    (void)filename;
    return 1;
}

void platform_graphics_unload_texture(int texture_id) {
    // For now, this is a no-op
    // In a full implementation, we'd unload the texture
    (void)texture_id;
}

#endif // GRAPHICS_BACKEND_RAYLIB