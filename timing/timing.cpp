#include "SDL_utils/SDL_wrapper.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <iostream>
#include "utils.h"
#include <string>
#include <sstream>

const std::string PRO_DIR(MACRO_PROJECT_DIR);
const int SCALE = 2;
const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

int main(int argc, char **argv) {
    try {
        SDL_Initializer sdl_initializer(SDL_INIT_VIDEO);
        TTF_Initializer ttf_initializer;
        WWindow window("Timing", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        WRenderer renderer(window.get(), -1, SDL_RENDERER_ACCELERATED);
        
        WTTFFont font (PRO_DIR + "/timing/lazy.ttf", 64);
        WTTFSurface surface(font.get(), "Press Enter to restart time", { 0, 0, 0, 255});
        WTexture texture(renderer.get(), surface.get());
        
        
        SDL_Event e;
        bool quit = false;
        Timer timer;
        timer.start();
        
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                } else if (e.type == SDL_KEYDOWN) {
                    switch(e.key.keysym.sym) {
                        case SDLK_RETURN:
                            timer.stop();
                            break;
                        case SDLK_SPACE:
                            if (timer.isPaused()) {
                                timer.unpause();
                            } else {
                                timer.pause();
                            }
                            break;
                        case SDLK_s:
                            timer.start();
                            break;
                        default:
                            break;
                    }
                }
            }
            
            std::stringstream time_text("");
            time_text << "Milliseconds since start time " << timer.get_ticks();
            WTTFSurface surface_text(font.get(), time_text.str().c_str(), { 0, 0, 0, 255});
            WTexture texture_text(renderer.get(), surface_text.get());
            
            SDL_SetRenderDrawColor(renderer.get(), 255,255,255,255);
            SDL_RenderClear(renderer.get());
            SDL_Rect src = {0, 0, texture.width, texture.height};
            SDL_Rect dst = {0, 0, texture.width, texture.height};
            SDL_Rect dst2 = {0, SCREEN_HEIGHT/2, texture.width, texture.height};
            
            SDL_RenderCopy(renderer.get(), texture.get(), NULL, &dst);
            SDL_RenderCopy(renderer.get(), texture_text.get(), NULL, &dst2);
            SDL_RenderPresent(renderer.get());
        }
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}