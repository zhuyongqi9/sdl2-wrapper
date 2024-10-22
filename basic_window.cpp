#include <algorithm>
#include <iostream>
#include "SDL_wrapper/wrapper.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int argc, char **argv) {
    try {
        SDL_Initializer sdl_initializer(SDL_INIT_VIDEO);
        std::unique_ptr<WWindow> window(new WWindow("Baseic Window", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
        std::unique_ptr<WRenderer> renderer( window->create_renderer(-1, SDL_RENDERER_ACCELERATED));
        
        SDL_Event e;
    
        bool quit = false;
        while (!quit) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }
            
            renderer->set_color(255, 255, 255, 255);
            renderer->clear();
            renderer->present();
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

}
