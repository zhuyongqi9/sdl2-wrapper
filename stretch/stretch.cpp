#include <cstddef>
#include <iostream>
#include <SDL_wrapper/wrapper.h>
#include <string>
#include <vector>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const std::string PRO_DIR(MACRO_PROJECT_DIR);

int main(int argc, char **argv) {
    try {
        SDL_Initializer sdl_initializer(SDL_INIT_AUDIO);
        std::unique_ptr<WWindow> window(new WWindow("", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
        std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED));
        std::shared_ptr<WTexture> stretch(renderer->create_texture(WBMPSurface(PRO_DIR + "/stretch/stretch.bmp")));
        
        bool quit = false;
        SDL_Event e;
    
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }         
            }
            
            renderer->set_color(255, 255, 255, 255);
            renderer->clear();
            stretch->render(nullptr, nullptr);
            renderer->present();
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
