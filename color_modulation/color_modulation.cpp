#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <exception>
#include <iostream>
#include "SDL_utils/SDL_wrapper.h"

const int SCALE = 2;
const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

int main(int argc, char **argv) {
    try {
        SDL_Initializer initialer(W_SDL_INIT_VIDEO | W_IMG_INIT_PNG);
        WWindow window("color modulation", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN); 
        WRenderer renderer(window.get(), -1, SDL_RENDERER_ACCELERATED);
        WTexture texture("/home/kirin-zhu/Projects/sdl2-snippets/color_modulation/colors.png", renderer.get());

        SDL_Event e;
        bool quit = false;

        SDL_Rect texture_rect = { 0, 0, texture.width, texture.height};
        SDL_Rect d_rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        uint8_t r,g,b;
        r = g = b = 255;
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                } else if (e.type == SDL_KEYDOWN) {
                    switch(e.key.keysym.sym) {
                        case SDLK_q:
                            r += 32;
                            break;
                        case SDLK_a:
                            r -= 32;
                            break;
                        case SDLK_w:
                            g += 32;
                            break;
                        case SDLK_s:
                            g -= 32;
                            break;
                        case SDLK_e:
                            b += 32;
                            break;
                        case SDLK_d:
                            b -= 32;
                            break;
                    }
                }
            }
            SDL_SetRenderDrawColor(renderer.get(), 0x00, 0x00, 0x00, 0x00);
            SDL_RenderClear(renderer.get());
            SDL_SetTextureColorMod(texture.get(), r, g, b);
            SDL_RenderCopy(renderer.get(), texture.get(), &texture_rect, &d_rect);
            SDL_RenderPresent(renderer.get());
        }

    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}