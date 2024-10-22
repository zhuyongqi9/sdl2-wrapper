
#include <iostream>
#include <string>
#include <vector>
#include "SDL_wrapper/wrapper.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const std::string PRO_DIR(MACRO_PROJECT_DIR);


class Widget{
public:
    Widget(WRenderer *renderer) {
        arr_texture.resize(KEY_PRESS_SURFACE_TOTAL);
        WBMPSurface surface_default(PRO_DIR + "/handle_key_event/press.bmp");
        arr_texture[KEY_PRESS_SURFACE_DEFAULT].reset(renderer->create_texture(&surface_default));
        WBMPSurface surface_down(PRO_DIR + "/handle_key_event/down.bmp");
        arr_texture[KEY_PRESS_SURFACE_DOWN].reset(renderer->create_texture(&surface_down));
        WBMPSurface surface_up(PRO_DIR + "/handle_key_event/up.bmp");
        arr_texture[KEY_PRESS_SURFACE_UP].reset(renderer->create_texture(&surface_up));
        WBMPSurface surface_left(PRO_DIR + "/handle_key_event/left.bmp");
        arr_texture[KEY_PRESS_SURFACE_LEFT].reset(renderer->create_texture(&surface_left));
        WBMPSurface surface_right(PRO_DIR + "/handle_key_event/right.bmp");
        arr_texture[KEY_PRESS_SURFACE_RIGHT].reset(renderer->create_texture(&surface_right));
        current = arr_texture[KEY_PRESS_SURFACE_DEFAULT].get();
    }

    void handle_event(SDL_Event *e) {
        if (e->type == SDL_KEYDOWN) {
            switch(e->key.keysym.sym) {
                case SDLK_UP:
                    current = arr_texture[KEY_PRESS_SURFACE_UP].get();
                    break;
                case SDLK_DOWN:
                    current = arr_texture[KEY_PRESS_SURFACE_DOWN].get();
                    break;
                case SDLK_LEFT:
                    current = arr_texture[KEY_PRESS_SURFACE_LEFT].get();
                    break;
                case SDLK_RIGHT:
                    current = arr_texture[KEY_PRESS_SURFACE_RIGHT].get();
                    break;
                default:
                    current = arr_texture[KEY_PRESS_SURFACE_DEFAULT].get();
                    break;
            }
        }
    }

    void render() {
        current->render(nullptr, nullptr);
    }
private:
    enum KeyPressSurfaces {
        KEY_PRESS_SURFACE_DEFAULT,
        KEY_PRESS_SURFACE_UP,
        KEY_PRESS_SURFACE_DOWN,
        KEY_PRESS_SURFACE_LEFT,
        KEY_PRESS_SURFACE_RIGHT,
        KEY_PRESS_SURFACE_TOTAL
    };

    std::vector<std::unique_ptr<WTexture>> arr_texture;
    WTexture *current;
};

int main(int argc, char **argv) {
    try {
        SDL_Initializer sdl_initializer(SDL_INIT_AUDIO);
        std::unique_ptr<WWindow> window(new WWindow("Baseic Window", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
        std::unique_ptr<WRenderer> renderer( window->create_renderer(-1, SDL_RENDERER_ACCELERATED));

        Widget widget(renderer.get());

        SDL_Event e;
        bool quit = false;
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                    break;
                }
                widget.handle_event(&e);
            }

            renderer->set_color(255, 255, 255, 255);
            renderer->clear();

            widget.render();

            renderer->present();
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
