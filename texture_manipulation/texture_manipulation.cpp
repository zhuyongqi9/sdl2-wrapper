#include <exception>
#include <iostream>
#include <SDL_wrapper/wrapper.h>

const std::string PRO_DIR(MACRO_PROJECT_DIR);

const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

int main(int argc, char **argv) {
    try {
        SDL_Initializer sdl_initializer(SDL_INIT_VIDEO);
        IMG_Initializer img_intializer(IMG_INIT_PNG);
        std::unique_ptr<WWindow> window(new WWindow("Texture Manipulation", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE |SDL_WINDOW_SHOWN));
        std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED));
        
        WPNGSurface surface_png(PRO_DIR + "/texture_manipulation/foo.png");
        surface_png.set_color_key(255, 0, 255);
        std::unique_ptr<WTexture> texture(renderer->create_texture(&surface_png));
        
        SDL_Event e;
        bool quit = false;
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }
            
            renderer->set_color(255, 255, 255, 255);
            renderer->clear();
            SDL_Rect dst = { 0, 0, texture->width, texture->height };
            texture->render(nullptr, &dst);
            renderer->present();
        }
        
    }catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}