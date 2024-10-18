#include <SDL_wrapper/wrapper.h>
#include <iostream>

const std::string PRO_DIR(MACRO_PROJECT_DIR);

const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

int main() {
    try {
        SDL_Initializer sdl_initializer(SDL_INIT_AUDIO);
        TTF_Initializer ttf_initializer;
        
        std::unique_ptr<WWindow> window(new WWindow("Texture PNG", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
        std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED));

        std::shared_ptr<WTTFFont> font(std::make_shared<WTTFFont>(PRO_DIR + "/ttf/lazy.ttf", 36));
        WTTFSurface surface_text(font.get(), "hello text", {0,0,0, 255});
        std::unique_ptr<WTexture> text(renderer->create_texture(&surface_text));
        
        
        SDL_Event e;
        bool quit = false;
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }

            renderer->set_color(0xFF, 0xFF, 0xFF, 0xFF);
            renderer->clear();
            
            SDL_Rect dst = {0, 0, text->width, text->height};
            text->render(nullptr, &dst);
            
            renderer->present();
        }

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
