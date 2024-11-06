#include "SDL2/SDL_events.h"
#include "SDL2/SDL_touch.h"
#include "SDL_wrapper/wrapper.h"
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_rect.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

const std::string PRO_DIR(MACRO_PROJECT_DIR);

const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;

const int SCREEN_WIDTH = BUTTON_WIDTH * 2;
const int SCREEN_HEIGHT = BUTTON_HEIGHT;

class Button {
public:
    Button(WRenderer *renderer):renderer(renderer) {
        picture  = renderer->create_texture(WPNGSurface(PRO_DIR + "/handle_mouse_event/button.png"));
        status = BUTTON_SPRITE_MOUSE_OUT;
    }
    
    void render() {
        //render button
        renderer->set_color(0,255,255,255);
        SDL_Rect rect = {0, 0, BUTTON_WIDTH, BUTTON_HEIGHT};
        renderer->fill_rect(&rect);
        
        //render text
        SDL_Rect dst = { BUTTON_WIDTH, 0, BUTTON_WIDTH, BUTTON_HEIGHT};
        picture->render(&src_arr[status], &dst);
    }
    
    void handle_event(SDL_Event &e) {
        int x,y;
        if (e.type == SDL_MOUSEMOTION) {
            SDL_GetMouseState(&x, &y);
            if (0 <= x && x <= BUTTON_WIDTH 
            && 0 <= y && y <= BUTTON_HEIGHT) {
                status = BUTTON_SPRITE_MOUSE_OVER;
            } else {
                status = BUTTON_SPRITE_MOUSE_OUT;
            }
        } else if (e.type == SDL_MOUSEBUTTONUP) {
            SDL_GetMouseState(&x, &y);
            if (0 <= x && x <= BUTTON_WIDTH 
            && 0 <= y && y <= BUTTON_HEIGHT) {
                status = BUTTON_SPRITE_MOUSE_UP;
            }
            
        } else if (e.type == SDL_MOUSEBUTTONDOWN) {
            SDL_GetMouseState(&x, &y);
            if (0 <= x && x <= BUTTON_WIDTH 
            && 0 <= y && y <= BUTTON_HEIGHT) {
                status = BUTTON_SPRITE_MOUSE_DOWN;
            }
        }
    }

private:    
    WRenderer *renderer; 
    enum button_sprite {
        BUTTON_SPRITE_MOUSE_OUT,
        BUTTON_SPRITE_MOUSE_OVER,
        BUTTON_SPRITE_MOUSE_DOWN,
        BUTTON_SPRITE_MOUSE_UP,
        BUTTON_SPRITE_TOTAL,
    };
    
    button_sprite status;
    std::unique_ptr<WTexture> picture;
    std::vector<SDL_Rect> src_arr = {
        {0, 0, BUTTON_WIDTH, BUTTON_HEIGHT},
        {0, BUTTON_HEIGHT * 1, BUTTON_WIDTH, BUTTON_HEIGHT},
        {0, BUTTON_HEIGHT * 2, BUTTON_WIDTH, BUTTON_HEIGHT},
        {0, BUTTON_HEIGHT * 3, BUTTON_WIDTH, BUTTON_HEIGHT}
    };
};


int main(int argc, char **argv) {
    try {
        SDL_Initializer sdl_initializer(SDL_INIT_AUDIO);
        IMG_Initializer img_initializer(IMG_INIT_PNG);
        
        std::unique_ptr<WWindow> window(new WWindow("Mouse Event", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
        std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED));
        
        Button button(renderer.get());

        SDL_Event e;
        bool quit = false;
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
                button.handle_event(e);
            }
            
            renderer->set_color(0xFF, 0xFF, 0xFF, 0xFF);
            renderer->clear();
            button.render();
            renderer->present();
        }
        
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}