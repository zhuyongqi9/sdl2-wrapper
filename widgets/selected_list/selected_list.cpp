#include <iostream>
#include <vector>
#include <widgets/selected_list.h>

const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

int main() {
    SDL_Initializer sdl_initializer(SDL_INIT_AUDIO);
    TTF_Initializer ttf_initializer;
    
    std::unique_ptr<WWindow> window(new WWindow("Selected List", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
    std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED));
    
    bool quit = false;
    SDL_Event e;
    std::vector<std::string> list_items(0); 
    for (int i = 0; i < 10; i++) list_items.push_back("device " + std::to_string(i));
    SelectedList list(renderer.get(), list_items, {20, 20});
    
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            list.handle_event(e);
        }
        
        renderer->set_color(255, 255, 255, 255);
        renderer->clear();
        list.render();
        renderer->present();
    }
}