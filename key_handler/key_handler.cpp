#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include <vector>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

std::string base_dir = "/home/kirin-zhu/Projects/sdl2-snippets/key_handler";

enum KeyPressSurfaces {
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

SDL_Window* gWindow = NULL;

std::vector<SDL_Surface*> gKeyPressSurfaces(KEY_PRESS_SURFACE_TOTAL);

SDL_Surface* gCurrentSurface = NULL;
SDL_Surface* gScreenSurface = NULL;

bool init() {
    //can't call any SDL functions without initializing SDL first
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    } else {
        return true;
    }
}

bool loadMedia();

SDL_Surface* loadSurface(std::string path) {
    // Load image at specified path 
    SDL_Surface* loadSurface = SDL_LoadBMP(path.c_str());
    if (loadSurface == NULL) {
        std::cerr << "Unable to laod iamge " << path << " error: " << SDL_GetError() << std::endl;;
    }
    return loadSurface;
}

void close() {
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}

bool loadMedia() {
    bool success = true;

    if ((gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface(base_dir + "/press.bmp")) == NULL) {
        std::cerr << "Failed to load default image" << std::endl; 
        success = false;
    }

    if ((gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface(base_dir + "/up.bmp")) == NULL) {
        std::cerr << "Failed to load up image" << std::endl; 
        success = false;
    }
    
    if ((gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface(base_dir + "/down.bmp")) == NULL) {
        std::cerr << "Failed to load down image" << std::endl; 
        success = false;
    }

    if ((gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface(base_dir + "/left.bmp")) == NULL) {
        std::cerr << "Failed to load  left image" << std::endl; 
        success = false;
    }

    if ((gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface(base_dir + "/right.bmp")) == NULL) {
        std::cerr << "Failed to load right image" << std::endl; 
        success = false;
    }

    return success;
}

int main(int argc, char **argv) {
    if (!init()) {
        std::cerr << "Failed to initialize SDL, error: " << SDL_GetError() << std::endl;
        exit(0);
    }

    if (!loadMedia()) {
        exit(0);
    }

    gWindow = SDL_CreateWindow("Basic Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
        std::cerr << "Failed to create window, error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    gScreenSurface = SDL_GetWindowSurface(gWindow);
    SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0xFF, 0xFF, 0xFF));
    SDL_UpdateWindowSurface(gWindow);

    bool quit = false;
    SDL_Event e;
    gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym)
                {
                case SDLK_UP:
                    gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
                    break;
                case SDLK_DOWN:                
                    gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
                    break;
                case SDLK_LEFT:
                    gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
                    break;
                case SDLK_RIGHT:
                    gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
                    break;
                default:
                    gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
                    break;
                }
            }
        }

        SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);
        SDL_UpdateWindowSurface(gWindow);
    }
    return 0;
}
