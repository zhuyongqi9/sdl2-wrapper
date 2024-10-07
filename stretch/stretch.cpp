#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include <vector>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

std::string base_dir = "/home/kirin-zhu/Projects/sdl2-snippets/stretch";

SDL_Window* gWindow = NULL;

SDL_Surface* gStretchSurface = NULL;
SDL_Surface* gScreenSurface = NULL;

bool init() {
    //can't call any SDL functions without initializing SDL first
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    } else {
        return true;
    }
}

SDL_Surface* loadSurface(std::string path) {
    // Load image at specified path 
    SDL_Surface* loadSurface = SDL_LoadBMP(path.c_str());
    if (loadSurface == nullptr) {
        std::cerr << "Unable to laod iamge " << path << " error: " << SDL_GetError() << std::endl;;
    }

    SDL_Surface* optimizedSurface = nullptr;
    optimizedSurface = SDL_ConvertSurface(loadSurface, gScreenSurface->format, 0);
    if (optimizedSurface == nullptr) {
        std::cerr << "Failed to optimize iamge error: " << SDL_GetError() << std::endl; 
    } else {
        SDL_FreeSurface(loadSurface);
    }

    return optimizedSurface;
}

void close() {
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}

bool loadMedia() {
    bool success = true;

    if ((gStretchSurface = loadSurface(base_dir + "/stretch.bmp")) == NULL) {
        std::cerr << "Failed to load default image" << std::endl; 
        success = false;
    }

    return success;
}

int main(int argc, char **argv) {
    if (!init()) {
        std::cerr << "Failed to initialize SDL, error: " << SDL_GetError() << std::endl;
        exit(0);
    }


    gWindow = SDL_CreateWindow("Basic Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
        std::cerr << "Failed to create window, error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    gScreenSurface = SDL_GetWindowSurface(gWindow);

    if (!loadMedia()) {
        exit(0);
    }

    SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0xFF, 0xFF, 0xFF));
    SDL_UpdateWindowSurface(gWindow);

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }         
        }

        SDL_Rect stretchRect;
        stretchRect.x = 0;
        stretchRect.y = 0;
        stretchRect.w = SCREEN_WIDTH;
        stretchRect.h = SCREEN_HEIGHT;

        SDL_BlitScaled(gStretchSurface, NULL, gScreenSurface, &stretchRect);
        SDL_UpdateWindowSurface(gWindow);
    }
    return 0;
}
