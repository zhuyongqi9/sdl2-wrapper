#include "SDL_utils/SDL_wrapper.h"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_rect.h>
#include <exception>
#include <iostream>
#include <vector>

const int SCALE = 2;
const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

std::vector<SDL_Rect> sprite_rect_arr = {
    {0, 0, 64, 205},
    {64, 0, 64, 205},
    {128, 0, 64, 205},
    {192, 0, 64, 205},
};

const int FRAME = 15;

SDL_Rect dst = {0, 0, 64 * 3, 205 * 3};
int main(int argc, char **argv) {
    try {
        SDL_Initializer initializer(W_SDL_INIT_VIDEO | W_IMG_INIT_PNG);
        WWindow window("Animation", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        WRenderer renderer(window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        WTexture texture("/home/kirin-zhu/Projects/sdl2-snippets/animation/foo.png", renderer.get());

        bool quit = false;
        SDL_Event e;
        int frame = 0;
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                } else if (e.type == SDL_KEYDOWN){
                    switch ( e.key.keysym.sym ) {
                    }
                } 

            }

            SDL_SetRenderDrawColor(renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(renderer.get());
            
            SDL_RenderCopy(renderer.get(), texture.get(), &sprite_rect_arr[frame / FRAME], &dst);
            frame = (frame + 1) % (FRAME * sprite_rect_arr.size());

            SDL_RenderPresent(renderer.get());
        }

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}