#include <cstdint>
#include <iostream>
#include "SDL_wrapper/wrapper.h"
#include "utils.h"
#include <sstream>
#include <chrono>
#include <thread>

const std::string PRO_DIR(MACRO_PROJECT_DIR);

const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

const int FPS = 60;
const double TIME_PER_FRAME = 1000.0 / FPS;

class FrameCounter {
public:    
    FrameCounter(WRenderer *renderer):renderer(renderer) {
        cnt = 0;
        start_ticks = 0;
        font.reset(new WTTFFont(PRO_DIR + "/frame_rate_cap/lazy.ttf", 60));
    }
    
    void start() {timer.start(); start_ticks = timer.get_ticks();}
    
    double get_fps() {
        return fps;
    };
    
    void render() {
        double fps_2 = ((int)(fps));
        std::stringstream str;
        str << "FPS: " << fps_2;
        std::unique_ptr<WTexture> fps_text(renderer->create_texture(WTTFSurface(font.get(), str.str(), {0, 255, 0, 255})));
        SDL_Rect dst = {SCREEN_WIDTH - fps_text->width, 0, fps_text->width, fps_text->height};
        fps_text->render(nullptr, &dst);
    }
    
    void add_cnt() { 
        cnt+=1; 
        uint64_t ticks = timer.get_ticks();
        fps = (cnt / ((double)ticks - start_ticks)) * 1000;
    }
private:
    uint64_t cnt; 
    uint64_t start_ticks; 
    Timer timer;
    WRenderer *renderer;
    double fps;
    std::unique_ptr<WTTFFont> font;
};

int main(int argc, char **argv) {
    try {
        SDL_Initializer sdl_initializer(SDL_INIT_AUDIO);
        TTF_Initializer ttf_initializer;
        
        std::unique_ptr<WWindow> window(new WWindow("Frame Counter", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
        std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED));
        
        FrameCounter counter(renderer.get());
        
        SDL_Event e;
        bool quit = false;
        
        Timer timer;
        timer.start();
        
        counter.start();
        while (!quit) {
            uint64_t start = timer.get_ticks();
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }
            renderer->set_color(0, 0, 0, 255);
            renderer->clear();
            counter.render();
            renderer->present();
            counter.add_cnt();
            
            uint64_t end = timer.get_ticks();
            if ((end - start) < (TIME_PER_FRAME)) {
                std::this_thread::sleep_for(std::chrono::milliseconds((int)(TIME_PER_FRAME - (end-start))));
            }
        }
        
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}