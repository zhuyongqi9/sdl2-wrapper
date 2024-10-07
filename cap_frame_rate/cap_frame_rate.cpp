#include <SDL2/SDL_render.h>
#include <cstdint>
#include <iostream>
#include "SDL_utils/SDL_wrapper.h"
#include "utils.h"
#include <sstream>
#include <chrono>
#include <thread>

const std::string PRO_DIR(MACRO_PROJECT_DIR);
const int SCALE = 2;
const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

const int FPS = 60;
const int TICKS_PRE_FRAME= 1000 / FPS;

int main(int argc, char **argv) {
    try {
        SDL_Initializer sdl_initializer(SDL_INIT_VIDEO);
        TTF_Initializer ttf_initializer;
        WWindow window("Cap Frame Rate", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        WRenderer renderer(window.get(), -1, SDL_RENDERER_ACCELERATED);
        WTTFFont font(PRO_DIR + "/cap_frame_rate/lazy.ttf", 64);
        
        SDL_Event e;
        bool quit = false;
        double rate;
        uint64_t total_frames;
        uint64_t ticks_count;
        Timer timer;
        timer.start();
        
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }
            uint64_t start = timer.get_ticks();
            rate = total_frames / (timer.get_ticks() / 1000.0);
            std::stringstream ss("");
            ss << "Averate FPS is " << rate;
            WTTFSurface surface(font.get(), ss.str().c_str(), {0, 0, 0, 255});
            WTexture texture(renderer.get(), surface.get());
            
            SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 255);
            SDL_RenderClear(renderer.get());
            SDL_Rect dst = { 0, 0, texture.width, texture.height};
            SDL_RenderCopy(renderer.get(), texture.get(), NULL, &dst);
            SDL_RenderPresent(renderer.get());
            total_frames++;
            
            uint64_t end = timer.get_ticks();
            if ((end - start) < TICKS_PRE_FRAME) {
                std::this_thread::sleep_for(std::chrono::milliseconds(TICKS_PRE_FRAME - (end-start)));
            }
        }
        
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}