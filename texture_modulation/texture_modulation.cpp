#include <exception>
#include <iostream>
#include "SDL_wrapper/wrapper.h"

const std::string PRO_DIR(MACRO_PROJECT_DIR);

const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

class ColorBoard {
public:
    ColorBoard(WRenderer *renderer):renderer(renderer) {
        picture  = renderer->create_texture(WPNGSurface(PRO_DIR + "/texture_modulation/colors.png"));
        r = g = b = 255;
    }
    
    void handle_event(SDL_Event &e) {
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_q:
                    r += 32;
                    break;
                case SDLK_a:
                    r -= 32;
                    break;
                case SDLK_w:
                    g += 32;
                    break;
                case SDLK_s:
                    g -= 32;
                    break;
                case SDLK_e:
                    b += 32;
                    break;
                case SDLK_d:
                    b -= 32;
                    break;
            }
        }
    } 
    
    void render() {
        picture->set_color_mod(r, g, b);
        picture->render(nullptr, nullptr);
    }
private:
    std::unique_ptr<WTexture> picture;
    WRenderer *renderer;
    int r,g,b;
};

int main(int argc, char **argv) {
    try {
        SDL_Initializer sdl_initializer(SDL_INIT_AUDIO);
        IMG_Initializer img_initializer(IMG_INIT_PNG);
        
        std::unique_ptr<WWindow> window(new WWindow("Texture Modulation", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
        std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED));

        ColorBoard board(renderer.get());
        
        SDL_Event e;
        bool quit = false;
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT){
                    quit = true;
                }
                board.handle_event(e);
            }
            
            renderer->set_color(0, 0, 0, 255);
            renderer->clear();
            board.render();
            renderer->present();
        }

    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}