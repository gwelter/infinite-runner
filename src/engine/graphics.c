#include "graphics.h"

// Forward declarations for platform-specific implementations
extern void platform_graphics_init(int width, int height, const char* title);
extern void platform_graphics_shutdown(void);
extern bool platform_graphics_should_close(void);
extern void platform_graphics_begin_frame(void);
extern void platform_graphics_end_frame(void);
extern void platform_graphics_clear(GfxColor color);
extern void platform_graphics_draw_rectangle(GfxRectangle rect, GfxColor color);
extern void platform_graphics_draw_texture(int texture_id, GfxRectangle dest, GfxColor tint);
extern void platform_graphics_draw_text(const char* text, int x, int y, int size, GfxColor color);
extern int platform_graphics_load_texture(const char* filename);
extern void platform_graphics_unload_texture(int texture_id);

static GraphicsBackend current_backend = GRAPHICS_RAYLIB;

void graphics_init(int width, int height, const char* title, GraphicsBackend backend) {
    current_backend = backend;
    platform_graphics_init(width, height, title);
}

void graphics_shutdown(void) {
    platform_graphics_shutdown();
}

bool graphics_should_close(void) {
    return platform_graphics_should_close();
}

void graphics_begin_frame(void) {
    platform_graphics_begin_frame();
}

void graphics_end_frame(void) {
    platform_graphics_end_frame();
}

void graphics_clear(GfxColor color) {
    platform_graphics_clear(color);
}

void graphics_draw_rectangle(GfxRectangle rect, GfxColor color) {
    platform_graphics_draw_rectangle(rect, color);
}

void graphics_draw_texture(int texture_id, GfxRectangle dest, GfxColor tint) {
    platform_graphics_draw_texture(texture_id, dest, tint);
}

void graphics_draw_text(const char* text, int x, int y, int size, GfxColor color) {
    platform_graphics_draw_text(text, x, y, size, color);
}

int graphics_load_texture(const char* filename) {
    return platform_graphics_load_texture(filename);
}

void graphics_unload_texture(int texture_id) {
    platform_graphics_unload_texture(texture_id);
}