#include <SDL_wrapper/wrapper.h>
#include <exception>
#include <iostream>

const std::string PRO_DIR(MACRO_PROJECT_DIR);

const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

class Arrow {
public:
    Arrow(WRenderer *renderer):renderer(renderer) {
        arrow.reset(renderer->create_texture(WPNGSurface(PRO_DIR + "/texture_rotation/arrow.png")));
        angle = 0.0;
    } 
    
    void render() {
        SDL_Point center = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
        arrow->renderEx(nullptr, nullptr, angle, &center, SDL_FLIP_NONE);
    }
    
    void handle_event(SDL_Event &e) {
        if (e.type == SDL_KEYDOWN) {
            switch(e.key.keysym.sym) {
                case SDLK_UP:
                    angle += 10.0;   
                    break;
                case SDLK_DOWN:
                    angle -= 10.0;   
                    break;
            }
        }
    }
private:
    std::unique_ptr<WTexture> arrow;
    WRenderer *renderer;
    double angle;
};
int main(int argc, char **argv) {
    try {
        SDL_Initializer sdl_initializer(SDL_INIT_AUDIO);
        IMG_Initializer img_initializer(IMG_INIT_PNG);
        
        std::unique_ptr<WWindow> window(new WWindow("Texture PNG", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
        std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED));
        
        Arrow arrow(renderer.get());
        
        bool quit = false;
        SDL_Event e;
        int frame = 0;

        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                } 
                arrow.handle_event(e);
            }

            renderer->set_color(0x00, 0x00, 0x00, 0xFF);
            renderer->clear();
            arrow.render();
            renderer->present();
        }

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}