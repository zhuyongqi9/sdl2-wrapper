#include <SDL_wrapper/wrapper.h>
#include <chrono>
#include <thread>
#include <utils.h>
#include <exception>
#include <iostream>
#include <vector>

const std::string PRO_DIR(MACRO_PROJECT_DIR);

const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;


const int FPS = 20;

class Foo {
public:    
    Foo(WRenderer *renderer): renderer(renderer) {
        WPNGSurface surface(PRO_DIR + "/animation/foo.png");
        surface.set_color_key(0, 255, 255);
        picture.reset(renderer->create_texture(&surface));
        timer.start();
    }
    
    void render() {
        uint64_t start = timer.get_ticks();
        SDL_Rect dst = {0, 0, widht, height};
        picture->render(&sprite_rect_arr[state], &dst);
        uint64_t end = timer.get_ticks();
        
        if (end - start < 1000 / FPS) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000/FPS - (end-start)));
        }
        state = (state + 1 ) % 4;
    }
    
private:
    std::unique_ptr<WTexture> picture;
    WRenderer *renderer;        
    SDL_Rect dst;
    int widht = 64;
    int height = 205;
    uint state = 0;
    Timer timer;
    std::vector<SDL_Rect> sprite_rect_arr = {
        {0, 0, 64, 205},
        {64, 0, 64, 205},
        {128, 0, 64, 205},
        {192, 0, 64, 205},
    };
};

int main(int argc, char **argv) {
    try {
        std::unique_ptr<WWindow> window(new WWindow("Animation", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
        std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED));

        Timer timer;
        timer.start();
        
        Foo foo(renderer.get());
        
        bool quit = false;
        SDL_Event e;
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                } 
            }

            renderer->set_color(0xFF, 0xFF, 0xFF, 0xFF);
            renderer->clear();
            foo.render();
            renderer->present();
        }

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}