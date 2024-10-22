#include <iostream>
#include <SDL_wrapper/wrapper.h>
#include <string>
#include <vector>

const std::string PRO_DIR(MACRO_PROJECT_DIR);


const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

int main(int argc, char **argv) {
    SDL_Initializer sdl_initializer(SDL_INIT_VIDEO);
    IMG_Initializer img_initializer(IMG_INIT_PNG);
    
    std::unique_ptr<WWindow> window(new WWindow("Viewport", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
    std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED));
    std::shared_ptr<WTexture> picture(renderer->create_texture(WPNGSurface(PRO_DIR + "/renderer_viewport/viewport.png")));

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }         
        }

        renderer->set_color(0x00, 0x00, 0x00, 0x00);
        renderer->clear();

        renderer->set_viewport({0, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT/2});
        picture->render( nullptr, nullptr);

        renderer->present();
    }
    return 0;
}
