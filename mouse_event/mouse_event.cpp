#include "SDL_utils/SDL_wrapper.h"
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_rect.h>
#include <iostream>
#include <vector>


const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;

const int SCREEN_WIDTH = BUTTON_WIDTH * 2;
const int SCREEN_HEIGHT = BUTTON_HEIGHT * 2;

enum button_sprite {
    BUTTON_SPRITE_MOUSE_OUT,
    BUTTON_SPRITE_MOUSE_OVER_DOWN,
    BUTTON_SPRITE_MOUSE_DOWN,
    BUTTON_SPRITE_MOUSE_UP,
    BUTTON_SPRITE_TOTAL,
};

std::vector<SDL_Rect> src_arr = {
    {0, 0, BUTTON_WIDTH, BUTTON_HEIGHT},
    {0, BUTTON_HEIGHT * 1, BUTTON_WIDTH, BUTTON_HEIGHT},
    {0, BUTTON_HEIGHT * 2, BUTTON_WIDTH, BUTTON_HEIGHT},
    {0, BUTTON_HEIGHT * 3, BUTTON_WIDTH, BUTTON_HEIGHT}
};

std::vector<SDL_Rect> dst_arr = {
    {0, 0, BUTTON_WIDTH, BUTTON_HEIGHT},
    {BUTTON_WIDTH, 0, BUTTON_WIDTH, BUTTON_HEIGHT},
    {0, BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT},
    {BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT},
};

std::vector<button_sprite> status = {
    BUTTON_SPRITE_MOUSE_OUT,
    BUTTON_SPRITE_MOUSE_OUT,
    BUTTON_SPRITE_MOUSE_OUT,
    BUTTON_SPRITE_MOUSE_OUT,
};

int main(int argc, char **argv) {
    try {
        SDL_Initializer initializer(W_SDL_INIT_VIDEO | W_IMG_INIT_PNG);
        WWindow window("Mouse Event", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        WRenderer renderer(window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        WPNGSurface surface("/home/kirin-zhu/Projects/sdl2-snippets/mouse_event/button.png");
        WTexture texture(renderer.get(), surface.get());

        SDL_Event e;
        bool quit = false;
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                } else if(e.type == SDL_MOUSEMOTION) {
                    int x,y;
                    SDL_GetMouseState(&x, &y);
                    std::cout << x << " " << y << std::endl;
                }
            }

            SDL_SetRenderDrawColor(renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(renderer.get());
            
            SDL_RenderCopy(renderer.get(), texture.get(), &src_arr[0], &dst_arr[0]);
            SDL_RenderCopy(renderer.get(), texture.get(), &src_arr[1], &dst_arr[1]);
            SDL_RenderCopy(renderer.get(), texture.get(), &src_arr[2], &dst_arr[2]);
            SDL_RenderCopy(renderer.get(), texture.get(), &src_arr[3], &dst_arr[3]);
            SDL_RenderPresent(renderer.get());
        }
        
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}