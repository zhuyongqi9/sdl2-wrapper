#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>

const int SCALE = 2;
const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

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


    SDL_UpdateWindowSurface(gWindow);

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }         
        }

        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(gRenderer);
        SDL_Rect fillRect = {SCREEN_WIDTH/4, SCREEN_HEIGHT/4, SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
        SDL_RenderFillRect(gRenderer, &fillRect);

        SDL_Rect outlineRect = { SCREEN_WIDTH/6, SCREEN_HEIGHT/6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3};
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
        SDL_RenderDrawRect(gRenderer, &outlineRect);

        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
        SDL_RenderDrawLine(gRenderer, 0 , SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT /2);

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0xFF);
        for (int i = 0; i < SCREEN_HEIGHT; i += 4) {
            SDL_RenderDrawPoint(gRenderer, SCREEN_WIDTH/2, i);
        }

        SDL_RenderPresent(gRenderer);

    }
    return 0;
}
