#include "SDL_utils/SDL_wrapper.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <cstdint>
#include <exception>
#include <iostream>

const int SCALE = 2;
const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

int main(int argc, char **argv) {
    try {
        SDL_Initializer initializer(W_SDL_INIT_VIDEO | W_IMG_INIT_PNG);
        WWindow window("Alpha Blending", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        WRenderer renderer(window.get(), -1, SDL_RENDERER_ACCELERATED);
        WPNGTexture front_texture("/home/kirin-zhu/Projects/sdl2-snippets/alpha_blending/fadeout.png", renderer.get());
        WPNGTexture back_texture("/home/kirin-zhu/Projects/sdl2-snippets/alpha_blending/fadein.png", renderer.get());

        bool quit = false;
        SDL_Event e;
        uint8_t front_alpha = 255;
        uint8_t back_alpha = 255;

        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                } else if (e.type == SDL_KEYDOWN){
                    switch ( e.key.keysym.sym ) {
                        case SDLK_w:
                            front_alpha += 10;
                            break;
                        case SDLK_s:
                            front_alpha -= 10;
                            break;
                        case SDLK_q:
                            back_alpha += 10;
                            break;
                        case SDLK_a:
                            back_alpha -= 10;
                            break;
                    }
                } 

            }

            SDL_SetRenderDrawColor(renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(renderer.get());

            SDL_SetTextureBlendMode( front_texture.get(), SDL_BLENDMODE_BLEND);
            SDL_SetTextureAlphaMod(front_texture.get(), front_alpha);
            SDL_RenderCopy(renderer.get(), front_texture.get(), NULL, NULL);

            SDL_SetTextureBlendMode(back_texture.get(), SDL_BLENDMODE_BLEND);
            SDL_SetTextureAlphaMod(back_texture.get(), back_alpha);
            SDL_RenderCopy(renderer.get(), back_texture.get(), NULL, NULL);

            SDL_RenderPresent(renderer.get());
        }

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}