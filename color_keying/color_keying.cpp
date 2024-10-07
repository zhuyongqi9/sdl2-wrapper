#include <SDL2/SDL_render.h>
#include <exception>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "SDL_utils/SDL_wrapper.h"

const int SCALE = 2;
const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

std::string base_dir = "";

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;


int main(int argc, char **argv) {
    try {
        SDL_Initializer inializer = SDL_Initializer(W_SDL_INIT_VIDEO | W_IMG_INIT_PNG);
        WWindow w_window("Basic Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        gWindow = w_window.get();

        WRenderer w_renderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
        gRenderer = w_renderer.get();

        bool quit = false;
        SDL_Event e;

        WTexture background_texture("/home/kirin-zhu/Projects/sdl2-snippets/color_keying/background.png", gRenderer);
        WTexture foo_texture("/home/kirin-zhu/Projects/sdl2-snippets/color_keying/foo.png", gRenderer);
        SDL_Rect foo_rect = { (int)(240 * 2.5), 190 * 4 - 130, foo_texture.width, foo_texture.height };

        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }         
            }
            SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
            SDL_RenderClear(gRenderer);
            SDL_RenderCopy(gRenderer, background_texture.get(), NULL, NULL);
            SDL_RenderCopy(gRenderer, foo_texture.get(), NULL, &foo_rect);
            SDL_RenderPresent(gRenderer);
        }

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;

}
