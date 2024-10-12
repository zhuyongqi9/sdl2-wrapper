#include <cinttypes>
#include <iostream>
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_surface.h"
#include "SDL2/SDL_video.h"
#include "SDL_utils/SDL_wrapper.h"

const std::string PRO_DIR(MACRO_PROJECT_DIR);

const int SCREEN_WIDTH = 640 * 2;
const int SCREEN_HEIGHT = 480 * 2;

SDL_Surface *surface_pixel;

uint32_t get_pixel(int x, int y) {
    uint32_t *pixels = static_cast<uint32_t *>(surface_pixel->pixels);
    return pixels[x + y * (static_cast<int32_t>(surface_pixel->pitch)/4)];
}

int find_left_edge(int x, int y, int width, int height, uint32_t bg_color) {
    for (int col = x; col < x + width; col++) {
        for (int row = y; row < y + height; row++) {
            if (get_pixel(col, row) != bg_color) {
                return col;
            }
        }
    }
    return -1;
}

int find_right_edge(int x, int y, int width, int height, uint32_t bg_color) {
    for (int col = x+width - 1; col >= x; col--) {
        for (int row = y; row < y + height; row++) {
            if (get_pixel(col, row) != bg_color) {
                return col + 1;
            }
        }
    }
    return -1;
}

int find_top_edge(int x, int y, int width, int height, uint32_t bg_color) {
    for (int row = y; row < y + height; row++) {
        for (int col = x; col < x + width; col++) {
            if (get_pixel(col, row) != bg_color) {
                return row;
            }
        }
    }
    return -1;
}

int find_bottom_edge(int x, int y, int width, int height, uint32_t bg_color) {
    for (int row = y + height -1 ; row >= y; row--) {
        for (int col = x; col < x + width; col++) {
            if (get_pixel(col, row) != bg_color) {
                return row;
            }
        }
    }
    return -1;
}

int main(int argc, char **argv) {
    try {
        SDL_Initializer sdl_initializer(SDL_INIT_VIDEO);
        IMG_Initializer img_initializer(IMG_INIT_PNG);
        WWindow window("bitmap fonts", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        WRenderer renderer(window.get(), -1, SDL_RENDERER_ACCELERATED);
        
        WPNGSurface surface(PRO_DIR + "/bitmap_fonts/lazyfont.png");
        surface_pixel = SDL_ConvertSurfaceFormat(surface.get(), SDL_GetWindowPixelFormat(window.get()), 0);
//        SDL_SetColorKey(surface_pixel, SDL_TRUE, SDL_MapRGB(surface_pixel->format, 0, 255, 255));
        
        uint32_t *pixels = static_cast<uint32_t *>(surface_pixel->pixels);
        WTexture texture(renderer.get(), surface_pixel);
        
        int cell_width = texture.width / 16;
        int cell_height = texture.height / 16;
        int current_char = 0;
        
        int left_edge = find_left_edge(0, cell_height*5, cell_width, cell_height, get_pixel(0, 0));
        int right_edge = find_right_edge(0, cell_height*5, cell_width, cell_height, get_pixel(0, 0));
        int top_edge = find_top_edge(0, cell_height*5, cell_width, cell_height, get_pixel(0, 0));
        int bottom_edge = find_bottom_edge(0, cell_height*5, cell_width, cell_height, get_pixel(0, 0));
        std::cout << top_edge << std::endl;
        std::cout << bottom_edge << std::endl;
        
        SDL_Event e;
        bool quit = false;
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }
            
            SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 255);
            SDL_RenderClear(renderer.get());
            SDL_Rect src = {left_edge, top_edge, right_edge - left_edge, bottom_edge - top_edge};
            SDL_Rect dst = {20, 20, right_edge - left_edge, bottom_edge - top_edge};
            SDL_RenderCopy(renderer.get(), texture.get(), &src, &dst);
            SDL_RenderPresent(renderer.get());
        }
        
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}