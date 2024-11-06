#include "SDL_wrapper/wrapper.h"
#include <cstdint>
#include <exception>
#include <iostream>

const std::string PRO_DIR(MACRO_PROJECT_DIR);

const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

class Front  {
public:
    Front(WRenderer *renderer): renderer(renderer){
        picture  = renderer->create_texture(WPNGSurface(PRO_DIR + "/texture_alpha/fadein.png"));
        alpha = 255;
    }
    
    void handle_event(SDL_Event &e) {
        if(e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_q:
                    alpha += 15;
                    break;
                case SDLK_a:
                    alpha -= 15;
                    break;
            }
        }
    }
    
    void render(){
        picture->set_alpha_mod(alpha);
        picture->render(nullptr, nullptr);
    }
private:
    std::unique_ptr<WTexture> picture;
    WRenderer *renderer;
    uint8_t alpha;
};

class Back {
public:
    Back(WRenderer *renderer): renderer(renderer){
        picture  = renderer->create_texture(WPNGSurface(PRO_DIR + "/texture_alpha/fadeout.png"));
        alpha = 255;
    }
    
    void handle_event(SDL_Event &e) {
        if(e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_w:
                    alpha += 15;
                    break;
                case SDLK_s:
                    alpha -= 15;
                    break;
            }
        }
    }
    
    void render(){
        picture->set_alpha_mod(alpha);
        picture->render(nullptr, nullptr);
    }
private:
    std::unique_ptr<WTexture> picture;
    WRenderer *renderer;
    uint8_t alpha;
    
};

int main(int argc, char **argv) {
    try {
        SDL_Initializer sdl_initializer(SDL_INIT_AUDIO);
        IMG_Initializer img_initializer(IMG_INIT_PNG);
        
        std::unique_ptr<WWindow> window(new WWindow("Texture Alpha", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
        std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED));

        Front front(renderer.get());
        Back back(renderer.get());
        
        bool quit = false;
        SDL_Event e;

        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                } 
                front.handle_event(e);
                back.handle_event(e);
            }

            renderer->set_color(0, 0, 0, 0);
            renderer->clear();
            front.render();
            back.render();
            renderer->present();
        }

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}