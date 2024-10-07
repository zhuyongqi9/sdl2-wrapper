#include "SDL_utils/SDL_wrapper.h"
#include <SDL2/SDL_events.h>
#include <iostream>

const int SCALE = 2;
const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

int main() {
    try {
        SDL_Initializer initializer(W_SDL_INIT_VIDEO | W_IMG_INIT_PNG | W_TTF_INIT);
        WWindow window("Animation", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        WRenderer renderer(window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        WTTFFont font("/home/kirin-zhu/Projects/sdl2-snippets/true_type_font/lazy.ttf", 64);
        WTTFSurface ttf_surface(font.get(), "hello SDL", {0, 0, 0});
        WTexture texture(renderer.get(), ttf_surface.get());

        SDL_Event e;
        bool quit = false;
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }

            SDL_SetRenderDrawColor(renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(renderer.get());
            
            SDL_Rect rect = {0, 0, ttf_surface.get()->w, ttf_surface.get()->h};
            SDL_RenderCopy(renderer.get(), texture.get(), NULL, &rect);
            SDL_RenderPresent(renderer.get());
        }

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
