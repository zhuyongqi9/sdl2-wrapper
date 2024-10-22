#include <pthread.h>

#include <iostream>
#include <SDL_wrapper/wrapper.h>
#include <string>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const std::string PRO_DIR(MACRO_PROJECT_DIR);

int main(int argc, char **argv) {
    SDL_Initializer sdl_initializer(SDL_INIT_AUDIO);
    IMG_Initializer img_initializer(IMG_INIT_PNG);
    
    std::unique_ptr<WWindow> window(new WWindow("Texture PNG", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
    std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED));
    std::shared_ptr<WTexture> picture(renderer->create_texture(WPNGSurface(PRO_DIR + "/texture_png/images.png")));
    
    SDL_Rect dst = {0, 0, picture->width, picture->height};
    
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
        picture->render(nullptr, &dst);
        renderer->present();
    }
    return 0;
}
