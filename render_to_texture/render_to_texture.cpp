#include <chrono>
#include <iostream>
#include <thread>
#include "SDL2/SDL_pixels.h"
#include "SDL2/SDL_render.h"
#include "SDL_Utils/SDL_wrapper.h"
#include "utils.h"

const std::string PRO_DIR(MACRO_PROJECT_DIR);

const int SCREEN_WIDTH = 640 ;
const int SCREEN_HEIGHT = 480 ;

const int TICKS_PER_FRAME = 1000 / 60;

int main() {
    SDL_Initializer sdl_initializer(SDL_INIT_VIDEO);
    IMG_Initializer img_initializer(IMG_INIT_PNG);
    
    WWindow window("bitmap fonts", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    WRenderer renderer(window.get(), -1, SDL_RENDERER_ACCELERATED);
    
    SDL_Texture *texture = SDL_CreateTexture(renderer.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    Timer timer;
    timer.start();
    
    SDL_Event e;
    bool quit = false;
    int angle = 0;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
        
        uint64_t start = timer.get_ticks();
        SDL_SetRenderTarget(renderer.get(), texture);
        SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);
        SDL_RenderClear(renderer.get());
        
        SDL_SetRenderDrawColor(renderer.get(), 255, 0, 0, 255);
        SDL_Rect fill_rect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
        SDL_RenderFillRect(renderer.get(), &fill_rect);
        
        SDL_SetRenderDrawColor(renderer.get(), 0, 255, 0, 255);
        SDL_Rect line_rect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2/ 3, SCREEN_HEIGHT * 2/ 3};
        SDL_RenderDrawRect(renderer.get(), &line_rect);
        
        SDL_SetRenderDrawColor(renderer.get(), 0, 0, 255, 255);
        SDL_RenderDrawLine(renderer.get(), 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);
        
        
        SDL_SetRenderTarget(renderer.get(), NULL);
        SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);
        SDL_RenderClear(renderer.get());
        
        SDL_Rect src = {0,0, SCREEN_WIDTH,SCREEN_HEIGHT};
//        SDL_Point point = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
        SDL_Point point = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4};
        SDL_RenderCopyEx(renderer.get(), texture, &src, &src, angle, &point, SDL_FLIP_NONE);
        SDL_RenderPresent(renderer.get());
        
        angle += 1;
        if (angle > 360 ) angle = 0;
        uint64_t end= timer.get_ticks();
        if (end - start < TICKS_PER_FRAME) {
            std::this_thread::sleep_for(std::chrono::milliseconds(TICKS_PER_FRAME - (end-start)));
        }
    }
}