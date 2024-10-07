#include "SDL_utils/SDL_wrapper.h"
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_rect.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

const std::string PROJECT_DIR(MACRO_PROJECT_DIR);

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
        WPNGSurface surface(PROJECT_DIR + "/mouse_event/button.png");
        WTexture texture(renderer.get(), surface.get());

        SDL_Event e;
        bool quit = false;
        int x, y;
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                } else if(e.type == SDL_MOUSEMOTION) {
                    SDL_GetMouseState(&x, &y);
                    std::fill(status.begin(), status.end(), BUTTON_SPRITE_MOUSE_OUT);
                    int area = x / BUTTON_WIDTH + (y / BUTTON_HEIGHT) * 2;
                    status[area] = BUTTON_SPRITE_MOUSE_OVER_DOWN;
                } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                    SDL_GetMouseState(&x, &y);
                    std::fill(status.begin(), status.end(), BUTTON_SPRITE_MOUSE_OUT);
                    int area = x / BUTTON_WIDTH + (y / BUTTON_HEIGHT) * 2;
                    status[area] = BUTTON_SPRITE_MOUSE_DOWN;
                } else if (e.type == SDL_MOUSEBUTTONUP) {
                    SDL_GetMouseState(&x, &y);
                    std::fill(status.begin(), status.end(), BUTTON_SPRITE_MOUSE_OUT);
                    int area = x / BUTTON_WIDTH + (y / BUTTON_HEIGHT) * 2;
                    status[area] = BUTTON_SPRITE_MOUSE_UP;
                }
            }
            
            SDL_SetRenderDrawColor(renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(renderer.get());
            
            for (int i = 0; i < status.size(); i++) {
                SDL_RenderCopy(renderer.get(), texture.get(), &src_arr[status[i]], &dst_arr[i]);
            }
            SDL_RenderPresent(renderer.get());
        }
        
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}