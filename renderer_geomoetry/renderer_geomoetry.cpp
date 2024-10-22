#include <iostream>
#include <SDL_wrapper/wrapper.h>
#include <string>
#include <vector>


const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;


int main(int argc, char **argv) {
    SDL_Initializer sdl_initializer(SDL_INIT_AUDIO);
    IMG_Initializer img_initializer(IMG_INIT_PNG);
    
    std::unique_ptr<WWindow> window(new WWindow("Geomoetry", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
    std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED));

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }         
        }

        renderer->set_color(0, 0, 0, 255);
        renderer->clear();
        SDL_Rect fillRect = {SCREEN_WIDTH/4, SCREEN_HEIGHT/4, SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
        renderer->set_color(0xFF, 0x00, 0x00, 0xFF);
        renderer->fill_rect(&fillRect);

        SDL_Rect outlineRect = { SCREEN_WIDTH/6, SCREEN_HEIGHT/6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3};
        renderer->set_color(0x00, 0xFF, 0x00, 0xFF);
        renderer->draw_rect( &outlineRect);

        renderer->set_color(0x00, 0x00, 0xFF, 0xFF);
        renderer->draw_line({0 , SCREEN_HEIGHT/2}, {SCREEN_WIDTH, SCREEN_HEIGHT /2});

        renderer->set_color(0xFF, 0xFF, 0x00, 0xFF);
        for (int i = 0; i < SCREEN_HEIGHT; i += 4) {
            renderer->draw_point({SCREEN_WIDTH/2, i});
        }

        renderer->present();
    }
    return 0;
}
