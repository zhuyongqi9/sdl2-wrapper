#include <iostream>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;


int main(int argc, char **argv) {
    SDL_Window* window = NULL;

    //2D image
    SDL_Surface* screenSurface = NULL;

    //can't call any SDL functions without initializing SDL first
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL, error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    window = SDL_CreateWindow("Basic Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        std::cerr << "Failed to create window, error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    screenSurface = SDL_GetWindowSurface(window);
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
    SDL_UpdateWindowSurface(window);

    SDL_Event e;

    bool flag = true;
    while (flag) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                flag = false;
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
