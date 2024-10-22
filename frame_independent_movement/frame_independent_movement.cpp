#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include "SDL2/SDL_pixels.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_video.h"
#include "SDL_Utils/SDL_wrapper.h"
#include "utils.h"

const std::string PRO_DIR(MACRO_PROJECT_DIR);

const int SCREEN_WIDTH = 640 ;
const int SCREEN_HEIGHT = 480 ;

const int FPS = 60;
const int TICKS_PER_FRAME = 1000 / FPS;

const float SPEED = 120.0 / FPS;

int main() {
    SDL_Initializer sdl_initializer(SDL_INIT_VIDEO);
    IMG_Initializer img_initializer(IMG_INIT_PNG);
    
    WWindow window("bitmap fonts", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    WRenderer renderer(window.get(), -1, SDL_RENDERER_ACCELERATED);
    
    WPNGSurface surface_png(PRO_DIR + "/frame_independent_movement/dot.bmp");
    WTexture texture(renderer.get(), surface_png.get());
    
    Timer timer;
    timer.start();
    
    float x = 0;
    
    SDL_Event e;
    bool quit = false;
    uint64_t dot_start = timer.get_ticks();
    int angle = 0;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
        
        uint64_t start = timer.get_ticks();
        SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 255);
        SDL_RenderClear(renderer.get());
        
        SDL_Rect dst = {static_cast<int>(x), 0, texture.width, texture.height};
        SDL_RenderCopy(renderer.get(), texture.get(), NULL, &dst);
        SDL_RenderPresent(renderer.get());
        
        
        x += SPEED;
        if (x > SCREEN_WIDTH) x = 0;
        uint64_t end= timer.get_ticks();
        if (end - start < TICKS_PER_FRAME) {
            std::this_thread::sleep_for(std::chrono::milliseconds(TICKS_PER_FRAME - (end-start)));
        }
    }
}