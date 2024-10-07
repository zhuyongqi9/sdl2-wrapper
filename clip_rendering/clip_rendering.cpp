#include "SDL_utils/SDL_wrapper.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <iostream>
#include <vector>

const int SCALE = 2;
const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

SDL_Window* gWindow;
SDL_Renderer* gRenderer;

const int SPRITE_WIDTH = 100;

std::vector<SDL_Rect> g_sprite_clips = {
    {0, 0, SPRITE_WIDTH, SPRITE_WIDTH},
    {SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_WIDTH},
    {0, SPRITE_WIDTH, SPRITE_WIDTH, SPRITE_WIDTH},
    {SPRITE_WIDTH, SPRITE_WIDTH, SPRITE_WIDTH, SPRITE_WIDTH},
};

std::vector<SDL_Rect> g_sprite_corner_pos = {
    {0, 0, SPRITE_WIDTH, SPRITE_WIDTH},
    {SCREEN_WIDTH - SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_WIDTH},
    {0, SCREEN_HEIGHT - SPRITE_WIDTH, SPRITE_WIDTH, SPRITE_WIDTH},
    {SCREEN_WIDTH - SPRITE_WIDTH, SCREEN_HEIGHT - SPRITE_WIDTH, SPRITE_WIDTH, SPRITE_WIDTH},
};

int main(int argc, char **argv) {
    try {
        SDL_Initializer inializer = SDL_Initializer(W_SDL_INIT_VIDEO | W_IMG_INIT_PNG);
        WWindow w_window("Basic Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        gWindow = w_window.get();

        WRenderer w_renderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
        gRenderer = w_renderer.get();

        bool quit = false;
        SDL_Event e;

        WPNGTexture sprites_texture("/home/kirin-zhu/Projects/sdl2-snippets/clip_rendering/dots.png", gRenderer, WRGB(0, 0xFF, 0xFF));

        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }         
            }
            SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
            SDL_RenderClear(gRenderer);
            for (int i = 0; i < g_sprite_clips.size(); i++) {
                SDL_RenderCopy(gRenderer, sprites_texture.get(), &g_sprite_clips[i], &g_sprite_corner_pos[i]);
            }
            SDL_RenderPresent(gRenderer);
        }

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}