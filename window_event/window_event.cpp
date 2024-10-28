#include <SDL_wrapper/wrapper.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include <cstdint>
#include <iostream>

const std::string PRO_DIR(MACRO_PROJECT_DIR);

const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

int main(int argc, char **argv) {
    try {
        SDL_Initializer sdl_initializer(SDL_INIT_VIDEO);
        TTF_Initializer ttf_initializer;
        std::unique_ptr<WWindow> window(new WWindow("Window Event", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE |SDL_WINDOW_SHOWN));
        std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED));
        WTTFFont font(PRO_DIR + "/window_event/OpenSans-Regular.ttf", 48);
        std::string state_text("ss");
        
        SDL_Event e;
        bool quit = false;
        while(!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                } else if (e.type == SDL_WINDOWEVENT) {
                    switch(e.window.event) {
                        case SDL_WINDOWEVENT_RESIZED: 
                        {
                            int32_t width = e.window.data1;
                            int32_t height = e.window.data2;
                            state_text = "Change size to " + std::to_string(width) + "x" + std::to_string(height);
                        }
                        break;
                        case SDL_WINDOWEVENT_MAXIMIZED:
                            state_text = "Maximize window";
                            break;
                        case SDL_WINDOWEVENT_MINIMIZED:
                            state_text = "Minimize window";
                            break;
                        case SDL_WINDOWEVENT_FOCUS_GAINED:
                            state_text = "get keyboard focus";
                            break;
                        case SDL_WINDOWEVENT_FOCUS_LOST:
                            state_text = "lost keyboard focus";
                            break;
                        case SDL_WINDOWEVENT_ENTER:
                            state_text = "mouse enter";
                            break;
                        case SDL_WINDOWEVENT_LEAVE:
                            state_text = "mouse leave";
                            break;
                        case SDL_WINDOWEVENT_RESTORED:
                            state_text = "resotre window";
                            break;
                        case SDL_WINDOWEVENT_EXPOSED:
                            state_text = "expose window";
                            break;
                    }
                    std::cout << state_text << std::endl;
                }
            }
            renderer->set_color( 255, 255, 255, 255);
            renderer->clear();
            WTTFSurfaceBlended surface_text(&font, state_text.c_str(), {0, 0, 0, 255});
            WTexture texture_text(renderer.get(), &surface_text);
            SDL_Rect dst = {0, 0, texture_text.width, texture_text.height};
            texture_text.render(nullptr, &dst);
            renderer->present();
        }
        
        
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}