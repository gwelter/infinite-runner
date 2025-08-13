#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdbool.h>

// Core graphics interface
typedef enum {
    GRAPHICS_RAYLIB,
    GRAPHICS_SDL3
} GraphicsBackend;

typedef struct {
    float x, y, width, height;
} GfxRectangle;

typedef struct {
    unsigned char r, g, b, a;
} GfxColor;

// Predefined colors
#define COLOR_WHITE     (GfxColor){255, 255, 255, 255}
#define COLOR_BLACK     (GfxColor){0, 0, 0, 255}
#define COLOR_RED       (GfxColor){255, 0, 0, 255}
#define COLOR_GREEN     (GfxColor){0, 255, 0, 255}
#define COLOR_BLUE      (GfxColor){0, 0, 255, 255}
#define COLOR_GRAY      (GfxColor){128, 128, 128, 255}

// Essential functions
void graphics_init(int width, int height, const char* title, GraphicsBackend backend);
void graphics_shutdown(void);
bool graphics_should_close(void);
void graphics_begin_frame(void);
void graphics_end_frame(void);
void graphics_clear(GfxColor color);
void graphics_draw_rectangle(GfxRectangle rect, GfxColor color);
void graphics_draw_texture(int texture_id, GfxRectangle dest, GfxColor tint);
void graphics_draw_text(const char* text, int x, int y, int size, GfxColor color);
int graphics_load_texture(const char* filename);
void graphics_unload_texture(int texture_id);

#endif // GRAPHICS_H