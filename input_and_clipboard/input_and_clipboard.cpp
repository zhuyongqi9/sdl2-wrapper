#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <exception>
#include <iostream>
#include "SDL_utils/SDL_wrapper.h"
#include <sstream>
#include "utils.h"

const std::string PRO_DIR(MACRO_PROJECT_DIR);
const int SCALE = 2;
const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

int main(int argc, char **argv) {
    try {
        SDL_Initializer sdl_initializer(SDL_INIT_VIDEO);
        TTF_Initializer ttf_initializer;
        WWindow window("Frame rate", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        WRenderer renderer(window.get(), -1, SDL_RENDERER_ACCELERATED);
        WTTFFont font(PRO_DIR + "/input_and_clipboard/lazy.ttf", 64);
        
        SDL_Event e;
        bool quit = false;
        SDL_StartTextInput();
        
        std::string s("");
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                } else if (e.type == SDL_TEXTINPUT) {
                    s += e.text.text ;
                } else if (e.type == SDL_KEYDOWN) {
                    SDL_Keycode code = e.key.keysym.sym;
                    if (code == SDLK_BACKSPACE) {
                        if (s.length() > 0)
                            s.pop_back(); 
                        std::cout << s << std::endl;
                    } else if (code == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
                        char *tmp_text = SDL_GetClipboardText(); 
                        s += tmp_text;
                        SDL_free(tmp_text);
                    } else if (code == SDLK_c && SDL_GetModState() & KMOD_CTRL) {
                        SDL_SetClipboardText(s.c_str());
                    }
                }
            }
        
            WTTFSurface surface(font.get(), "Input text: " + s, {0, 0, 0, 255});
            WTexture texture(renderer.get(), surface.get());
            
            SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 255);
            SDL_RenderClear(renderer.get());
            SDL_Rect dst = {0, 0, texture.width, texture.height};
            SDL_RenderCopy(renderer.get(), texture.get(), NULL, &dst);
            SDL_RenderPresent(renderer.get());
        }
        
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}