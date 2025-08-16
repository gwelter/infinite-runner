#ifdef GRAPHICS_BACKEND_SDL3

#include "../engine/graphics.h"
#include <SDL3/SDL.h>
#include <stdbool.h>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static bool should_close = false;

void platform_graphics_init(int width, int height, const char* title) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    window = SDL_CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == NULL) {
        SDL_Log("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
}

void platform_graphics_shutdown(void) {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = NULL;
    }
    SDL_Quit();
}

bool platform_graphics_should_close(void) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) {
            should_close = true;
        } else if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE) {
            should_close = true;
        }
    }
    return should_close;
}

void platform_graphics_begin_frame(void) {
    // SDL doesn't need explicit begin frame, but we can do setup here if needed
}

void platform_graphics_end_frame(void) {
    SDL_RenderPresent(renderer);
}

void platform_graphics_clear(GfxColor color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

void platform_graphics_draw_rectangle(GfxRectangle rect, GfxColor color) {
    SDL_FRect sdl_rect = {rect.x, rect.y, rect.width, rect.height};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &sdl_rect);
}

void platform_graphics_draw_texture(int texture_id, GfxRectangle dest, GfxColor tint) {
    // For now, we'll implement this as a placeholder
    // In a full implementation, we'd maintain a texture registry
    (void)texture_id;
    (void)dest;
    (void)tint;
}

void platform_graphics_draw_text(const char* text, int x, int y, int size, GfxColor color) {
    // SDL3 doesn't have built-in text rendering like Raylib
    // We'd need SDL_ttf or a similar library for proper text rendering
    // For now, this is a placeholder
    (void)text;
    (void)x;
    (void)y;
    (void)size;
    (void)color;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDebugText(renderer, x, y, text);
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

#endif // GRAPHICS_BACKEND_SDL3