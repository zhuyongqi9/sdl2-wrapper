#include <chrono>
#include <iostream>
#include <thread>
#include <SDL_wrapper/wrapper.h>
#include "utils.h"

const std::string PRO_DIR(MACRO_PROJECT_DIR);

const int SCREEN_WIDTH = 640 ;
const int SCREEN_HEIGHT = 480 ;

const int TICKS_PER_FRAME = 1000 / 60;

int main() {
    SDL_Initializer sdl_initializer(SDL_INIT_AUDIO);
    IMG_Initializer img_initializer(IMG_INIT_PNG);
    
    std::unique_ptr<WWindow> window(new WWindow("Render To Texture", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
    std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
    
    SDL_Texture *sdl_texture = SDL_CreateTexture(renderer->get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
    WTexture texture(renderer.get(), sdl_texture);
    
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
        renderer->set_target(&texture);
        renderer->set_color(0, 0, 0, 255);
        renderer->clear();
        
        SDL_Rect fill_rect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
        renderer->fill_rect(&fill_rect, {255, 0, 0, 255});
        
        SDL_Rect line_rect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2/ 3, SCREEN_HEIGHT * 2/ 3};
        renderer->draw_rect(&line_rect, {0, 255, 0, 255});
        
        renderer->set_color(0, 0, 255, 255);
        renderer->draw_line({0, SCREEN_HEIGHT / 2}, {SCREEN_WIDTH, SCREEN_HEIGHT / 2});
        
        renderer->set_target(nullptr);
        renderer->set_color(0, 0, 0, 255);
        renderer->clear();
        
        SDL_Rect src = {0,0, SCREEN_WIDTH,SCREEN_HEIGHT};
        SDL_Point point = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4};
        texture.renderEx(&src, &src, angle, &point, SDL_FLIP_NONE);
        renderer->present();
        
        angle += 1;
        if (angle > 360 ) angle = 0;
        uint64_t end= timer.get_ticks();
        if (end - start < TICKS_PER_FRAME) {
            std::this_thread::sleep_for(std::chrono::milliseconds(TICKS_PER_FRAME - (end-start)));
        }
    }
}