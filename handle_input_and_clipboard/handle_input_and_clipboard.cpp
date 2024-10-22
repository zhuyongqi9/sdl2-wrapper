#include <exception>
#include <iostream>
#include <SDL_wrapper/wrapper.h>
#include <memory>
#include "utils.h"

const std::string PRO_DIR(MACRO_PROJECT_DIR);

const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

class TextInput {
public:
    TextInput(WRenderer *renderer): renderer(renderer),
    font(PRO_DIR + "/handle_input_and_clipboard/lazy.ttf", 64) {
        s = "";
    }

    void handle_event(SDL_Event &e) {
        if (e.type == SDL_TEXTINPUT) {
            s += e.text.text;
        } else if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_BACKSPACE) {
                s.pop_back();
            } else if (e.key.keysym.sym == SDLK_v && (SDL_GetModState() & KMOD_CTRL)) {
                WClipboardText clipboard;
                s += clipboard.get();
            } else if (e.key.keysym.sym == SDLK_c && (SDL_GetModState() & KMOD_CTRL)) {
                SDL_SetClipboardText(s.c_str());
            }
        }
    }

    void render() {
        if (s.length() > 0) {
            WTexture text(renderer, WTTFSurfaceSolid(&font, s, {0, 0, 0, 255}, SCREEN_WIDTH));
            SDL_Rect dst = {0, 0, text.width, text.height};
            text.render(nullptr, &dst);
        }
    }

private:
    WRenderer *renderer;
    WTTFFont font;
    std::string s;
};

int main(int argc, char **argv) {
    try {
        SDL_Initializer sdl_initializer(SDL_INIT_AUDIO);
        TTF_Initializer ttf_initializer;

        std::unique_ptr<WWindow> window(new WWindow("Input & Clipboard", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
        std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC));

        SDL_Event e;
        bool quit = false;
        SDL_StartTextInput();
        TextInput input(renderer.get());

        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }

                input.handle_event(e);
            }

            renderer->set_color(255, 255, 255, 255);
            renderer->clear();
            input.render();
            renderer->present();
        }

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
