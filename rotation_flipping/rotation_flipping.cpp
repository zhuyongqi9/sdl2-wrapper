#include "SDL_utils/SDL_wrapper.h"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <exception>
#include <iostream>

const int SCALE = 2;
const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;


int main(int argc, char **argv) {
    try {
        SDL_Initializer initializer(W_SDL_INIT_VIDEO | W_IMG_INIT_PNG);
        WWindow window("Animation", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        WRenderer renderer(window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        WTexture texture("/home/kirin-zhu/Projects/sdl2-snippets/rotation_flipping/arrow.png", renderer.get());

        bool quit = false;
        SDL_Event e;
        int frame = 0;
        double angle = 0;
        SDL_RendererFlip flip_flag = SDL_FLIP_NONE;

        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                } else if (e.type == SDL_KEYDOWN){
                    switch ( e.key.keysym.sym ) {
                        case SDLK_q:
                            angle -= 10;
                            break;
                        case SDLK_e:
                            angle += 10;
                            break;
                        case SDLK_w:
                            flip_flag = SDL_RendererFlip (flip_flag & ~SDL_FLIP_HORIZONTAL);
                            break;
                        case SDLK_s:
                            flip_flag = (SDL_RendererFlip)(flip_flag | SDL_FLIP_HORIZONTAL);
                            break;
                        case SDLK_a:
                            flip_flag = SDL_RendererFlip (flip_flag & ~SDL_FLIP_VERTICAL);
                            break;
                        case SDLK_d:
                            flip_flag = (SDL_RendererFlip)(flip_flag | SDL_FLIP_VERTICAL);
                            break;
                    }
                } 

            }

            SDL_SetRenderDrawColor(renderer.get(), 0x00, 0x00, 0x00, 0xFF);
            SDL_RenderClear(renderer.get());
            
            SDL_RenderCopyEx(renderer.get(), texture.get(), NULL, NULL, angle, NULL, flip_flag);

            SDL_RenderPresent(renderer.get());
        }

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}