#include "SDL_utils/SDL_wrapper.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_scancode.h>
#include <iostream>
#include <vector>

const std::string PROJECT_DIR(MACRO_PROJECT_DIR);

const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;

const int SCREEN_WIDTH = BUTTON_WIDTH * 2;
const int SCREEN_HEIGHT = BUTTON_HEIGHT * 2;

enum state {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    DEFAULT,
    TOTAL
};


int main() {
    try {
        SDL_Initializer initializer(SDL_INIT_VIDEO);
        IMG_Initializer img_initializer(IMG_INIT_PNG);
        WWindow window("Mouse Event", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        WRenderer renderer(window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        WPNGSurface default_surface(PROJECT_DIR + "/key_state/press.png");
        WPNGSurface left(PROJECT_DIR + "/key_state/left.png");
        WPNGSurface right(PROJECT_DIR + "/key_state/right.png");
        WPNGSurface up(PROJECT_DIR + "/key_state/up.png");
        WPNGSurface down(PROJECT_DIR + "/key_state/down.png");

        WTexture texture_left(renderer.get(), left.get());
        WTexture texture_right(renderer.get(), right.get());
        WTexture texture_down(renderer.get(), down.get());
        WTexture texture_up(renderer.get(), up.get());
        WTexture texture_default(renderer.get(), default_surface.get());

        std::vector<SDL_Texture*> texture_arr = { 
            texture_up.get(),
            texture_down.get(),
            texture_left.get(),
            texture_right.get(),
            texture_default.get()
        };

        SDL_Event e;
        bool quit = false;
        state s = DEFAULT;
        while (!quit) {
            while(SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                } else if(e.type == SDL_KEYDOWN) {
                    switch(e.key.keysym.sym) {
                        case SDLK_UP:
                            s = UP;
                            break;
                        case SDLK_DOWN:
                            s = DOWN;
                            break;
                        case SDLK_LEFT:
                            s = LEFT;
                            break;
                        case SDLK_RIGHT:
                            s = RIGHT;
                            break;
                        default:
                            s = DEFAULT;
                            break;
                    }
                }


                SDL_SetRenderDrawColor(renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(renderer.get());
            
                SDL_RenderCopy(renderer.get(), texture_arr[s], NULL, NULL);
                SDL_RenderPresent(renderer.get());
            }
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
