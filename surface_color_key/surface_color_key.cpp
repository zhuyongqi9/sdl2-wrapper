#include <exception>
#include <iostream>
#include "SDL_wrapper/wrapper.h"

const std::string PRO_DIR(MACRO_PROJECT_DIR);


const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

class Background {
public:    
    Background(WRenderer *renderer):renderer(renderer) {
        WPNGSurface surface(PRO_DIR + "/surface_color_key/background.png");
        picture  = renderer->create_texture(&surface);
    }
    
    void render() {
        picture->render(nullptr, nullptr);
    }
private:
    std::unique_ptr<WTexture> picture;
    WRenderer *renderer;        
};

class Foo {
public:    
    Foo(WRenderer *renderer): renderer(renderer) {
        WPNGSurface surface(PRO_DIR + "/surface_color_key/foo.png");
        surface.set_color_key(0, 255, 255);
        picture  = renderer->create_texture(&surface);
        dst = { (int)(240 * 2.5), 150 * 4 - 130, picture->width, picture->height};
    }
    
    void render() {
        picture->render(nullptr, &dst);
    }
    
    void handle_event(SDL_Event &e) {
        if (e.type == SDL_KEYDOWN) {
            switch(e.key.keysym.sym) {
                case SDLK_UP:
                    dst.y -= 10;
                    break;
                case SDLK_DOWN:
                    dst.y += 10;
                    break;
                case SDLK_LEFT:
                    dst.x -= 10;
                    break;
                case SDLK_RIGHT:
                    dst.x += 10;
                    break;
            }
        }
    }
private:
    std::unique_ptr<WTexture> picture;
    WRenderer *renderer;        
    SDL_Rect dst;
};

int main(int argc, char **argv) {
    try {
        SDL_Initializer sdl_initializer(SDL_INIT_VIDEO);
        IMG_Initializer img_initializer(IMG_INIT_PNG);
        
        std::unique_ptr<WWindow> window(new WWindow("Color key", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
        std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED));

        Background background(renderer.get());
        Foo foo(renderer.get());
        
        bool quit = false;
        SDL_Event e;
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }         
                foo.handle_event(e);
            }
            
            renderer->set_color(0x00, 0x00, 0x00,255);
            renderer->clear();
            background.render();
            foo.render();
            renderer->present();
        }

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;

}
