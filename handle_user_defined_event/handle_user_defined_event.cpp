#include "SDL2/SDL_events.h"
#include <pthread.h>

#ifndef SDL_WRAPPER_H
#define SDL_WRAPPER_H
#include <SDL_wrapper/wrapper.h>
#endif

#include <iostream>
#include <string>
#include <widgets/text.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const std::string PRO_DIR(MACRO_PROJECT_DIR);

enum user_defined_event {
    USER_Event,
    USER_EVENT_TOTAL,
};

int main(int argc, char **argv) {
    SDL_Initializer sdl_initializer(SDL_INIT_AUDIO);
    TTF_Initializer ttf_initializer;
    
    std::unique_ptr<WWindow> window(new WWindow("Handle user dedfined event", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
    std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED));
    
    uint32_t user_event_base = SDL_RegisterEvents(USER_EVENT_TOTAL);
    
    bool quit = false;
    SDL_Event e;
    
    std::shared_ptr<std::string> test = std::make_shared<std::string>("");
    BindText text(renderer.get(), test, {20, 20});
    
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == USER_Event + user_event_base) {
                *test = "Receive USER_Event";
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                SDL_Event event;
                event.type = USER_Event + user_event_base;
                SDL_PushEvent(&event);
            }
        }

        renderer->set_color(255, 255, 255, 255);
        renderer->clear();
        text.render();
        renderer->present();
    }
    return 0;
}