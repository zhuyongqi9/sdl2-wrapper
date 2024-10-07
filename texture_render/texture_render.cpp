#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

std::string base_dir = "";

SDL_Window* gWindow = nullptr;

SDL_Renderer* gRenderer = nullptr;
SDL_Texture* gTexture = nullptr;


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

SDL_Texture* loadTexture(std::string path) {

    SDL_Surface* loadSurface = IMG_Load(path.c_str());
    if (loadSurface == nullptr) {
        std::cerr << "failed to load surface. error: " << IMG_GetError() << std::endl; 
        return nullptr;
    } else {
        SDL_Texture* newTexture = NULL;
        if ((newTexture = SDL_CreateTextureFromSurface(gRenderer, loadSurface)) == nullptr) {
            std::cerr << "Faield create texture. error: " << SDL_GetError() << std::endl;
        }

        SDL_FreeSurface(loadSurface);   
        return newTexture;
    }
}


void close() {
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;

    SDL_DestroyTexture(gTexture);
    gTexture = nullptr;
    
    SDL_DestroyRenderer(gRenderer);
    gRenderer = nullptr;

    IMG_Quit();
    SDL_Quit();
}

bool loadMedia() {
    bool success = true;

    if ((gTexture = loadTexture("/home/kirin-zhu/Projects/sdl2-snippets/texture_render/texture.png")) == NULL) {
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

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == nullptr) {
        std::cerr << "Failed to create renderer, error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);


    if (!loadMedia()) {
        exit(0);
    }

    SDL_UpdateWindowSurface(gWindow);

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }         
        }

        SDL_RenderClear(gRenderer);
        SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
        SDL_RenderPresent(gRenderer);
    }
    return 0;
}
