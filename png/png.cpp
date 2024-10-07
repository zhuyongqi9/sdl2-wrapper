#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

std::string base_dir = "/home/kirin-zhu/Projects/sdl2-snippets/png";

SDL_Window* gWindow = NULL;

SDL_Surface* gStretchSurface = NULL;
SDL_Surface* gScreenSurface = NULL;

bool init() {
    //can't call any SDL functions without initializing SDL first
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL, error: " << SDL_GetError() << std::endl;
        return false;
    }

    int img_flags = IMG_INIT_PNG;
    //only care about img_flag
    if (!(IMG_Init(img_flags) & img_flags)) {
        std::cerr << "Failed to initialize SDL, error: " << SDL_GetError() << std::endl;
        return false; 
    }
    return true;
}

SDL_Surface* loadSurface(std::string path) {
    // Load image at specified path 
    SDL_Surface* loadSurface = IMG_Load(path.c_str());
    if (loadSurface == nullptr) {
        std::cerr << "Unable to load iamge " << path << " error: " << IMG_GetError() << std::endl;;
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

SDL_Surface* loadPNGSurface(std::string path) {
    // Load image at specified path 
    SDL_Surface* loadSurface = IMG_Load(path.c_str());
    if (loadSurface == nullptr) {
        std::cerr << "Unable to laod iamge " << path << " error: " << IMG_GetError() << std::endl;;
    }

    SDL_Surface* optimizedSurface = nullptr;
    optimizedSurface = SDL_ConvertSurface(loadSurface, gScreenSurface->format, 0);
    if (optimizedSurface == nullptr) {
        std::cerr << "Failed to optimize iamge error: " << SDL_GetError() << std::endl; 
    } 
    
    SDL_FreeSurface(loadSurface);

    return optimizedSurface;
}

void close() {
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}

bool loadMedia() {
    bool success = true;

    if ((gStretchSurface = loadSurface(base_dir + "/images.png")) == NULL) {
        std::cerr << "Failed to load default image" << std::endl; 
        success = false;
    }

    return success;
}

int main(int argc, char **argv) {
    if (!init()) {
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
