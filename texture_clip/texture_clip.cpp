#include "SDL_wrapper/wrapper.h"
#include <iostream>
#include <vector>

const std::string PRO_DIR(MACRO_PROJECT_DIR);

const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

const int SPRITE_WIDTH = 100;

class RedDot {
public:   
    RedDot(WRenderer *renderer, std::shared_ptr<WTexture> texture): renderer(renderer), picture(texture) {
        src = {0, 0, SPRITE_WIDTH, SPRITE_WIDTH};
    }
    
    void render(SDL_Rect &&dst) {
        picture->render(&src, &dst);
    }
private:
    WRenderer *renderer;
    std::shared_ptr<WTexture> picture;
    SDL_Rect src;
};

class GreenDot {
public:   
    GreenDot(WRenderer *renderer, std::shared_ptr<WTexture> texture): renderer(renderer), picture(texture) {
        src = {SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_WIDTH};
    }
    void render(SDL_Rect &&dst) {
        picture->render(&src, &dst);
    }
private:
    WRenderer *renderer;
    std::shared_ptr<WTexture> picture;
    SDL_Rect src;

};

class YellowDot {
public:   
    YellowDot(WRenderer *renderer, std::shared_ptr<WTexture> texture): renderer(renderer), picture(texture) {
        src = {0, SPRITE_WIDTH, SPRITE_WIDTH, SPRITE_WIDTH};
    }
    void render(SDL_Rect &&dst) {
        picture->render(&src, &dst);
    }
private:
    WRenderer *renderer;
    std::shared_ptr<WTexture> picture;
    SDL_Rect src;

};

class BlueDot {
public:   
    BlueDot(WRenderer *renderer, std::shared_ptr<WTexture> texture): renderer(renderer), picture(texture) {
        src = {SPRITE_WIDTH, SPRITE_WIDTH, SPRITE_WIDTH, SPRITE_WIDTH};
    }
    void render(SDL_Rect &&dst) {
        picture->render(&src, &dst);
    }
private:
    WRenderer *renderer;
    std::shared_ptr<WTexture> picture;
    SDL_Rect src;
};

int main(int argc, char **argv) {
    try {
        SDL_Initializer sdl_initializer(SDL_INIT_AUDIO);
        IMG_Initializer img_initializer(IMG_INIT_PNG);
        
        std::unique_ptr<WWindow> window(new WWindow("Texture Clip", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
        std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED));
        WPNGSurface surface(PRO_DIR + "/texture_clip/dots.png");
        surface.set_color_key(0, 255, 255);
        std::shared_ptr<WTexture> picture(renderer->create_texture(&surface));
        
        
        bool quit = false;
        SDL_Event e;

        RedDot red(renderer.get(), picture);
        BlueDot blue(renderer.get(), picture);
        YellowDot yellow(renderer.get(), picture);
        GreenDot green(renderer.get(), picture);
        
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }         
            }
            
            renderer->set_color(0, 0, 0, 255);
            renderer->clear();
            red.render({0, 0, SPRITE_WIDTH, SPRITE_WIDTH});
            blue.render({300, 0, SPRITE_WIDTH, SPRITE_WIDTH});
            yellow.render({0, 300, SPRITE_WIDTH, SPRITE_WIDTH});
            green.render({300, 300, SPRITE_WIDTH, SPRITE_WIDTH});
            renderer->present();
        }

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}