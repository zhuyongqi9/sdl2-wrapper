#include <SDL2/SDL_events.h>
#include <iostream>
#include "SDL_utils/SDL_wrapper.h"
#include <sstream>
#include "utils.h"

const std::string PRO_DIR(MACRO_PROJECT_DIR);
const int SCALE = 2;
const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

int main(int argc, char **argv) {
    try {
        SDL_Initializer sdl_initializer(SDL_INIT_VIDEO);
        TTF_Initializer ttf_initializer;
        WWindow window("Frame rate", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        WRenderer renderer(window.get(), -1, SDL_RENDERER_ACCELERATED);
        WTTFFont font(PRO_DIR + "/frame_rate/lazy.ttf", 64);
        
        SDL_Event e;
        bool quit = false;
        double rate;
        Timer timer;
        timer.start();
        uint64_t total_frames;
        
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }
            
            rate = total_frames / (timer.get_ticks() / 1000.0);
            std::stringstream ss("");
            ss << "Averate FPS is " << rate;
            WTTFSurface surface(font.get(), ss.str().c_str(), {0, 0, 0, 255});
            WTexture texture(renderer.get(), surface.get());
            
            SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 255);
            SDL_RenderClear(renderer.get());
            SDL_Rect dst = {0, SCREEN_HEIGHT/2, texture.width, texture.height};
            SDL_RenderCopy(renderer.get(), texture.get(), NULL, &dst);
            SDL_RenderPresent(renderer.get());
            total_frames += 1;
        }
        
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}