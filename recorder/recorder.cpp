#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <chrono>
#include <thread>
#include <iostream>
#include "SDL_utils/SDL_wrapper.h"
#include <string>
#include <vector>
#include "utils.h"
#include <cmath>

const std::string PRO_DIR(MACRO_PROJECT_DIR);

const int SCREEN_HEIGHT = 600;
const int SCREEN_WIDTH = 1200;

WRenderer *renderer;

const int FPS = 144;

class Grid {
public:
    static const int GRID_LENGTH = 30;
    Grid() {
        SDL_Texture *texture = SDL_CreateTexture(renderer->get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
        SDL_SetRenderTarget(renderer->get(), texture);
        SDL_SetRenderDrawColor(renderer->get(), 105, 105, 105, 255);
        for (int i = 0; i < SCREEN_WIDTH ; i += GRID_LENGTH) {
            SDL_RenderDrawLine(renderer->get(), i, 0, i, SCREEN_HEIGHT);
        }
        
        for (int i = 0; i < SCREEN_HEIGHT; i += GRID_LENGTH) {
            SDL_RenderDrawLine(renderer->get(), 0, i, SCREEN_WIDTH, i);
        }
        SDL_SetRenderTarget(renderer->get(), NULL);
        this->texture = texture;
    }   
    ~Grid() {
        if (texture != nullptr) {
            SDL_DestroyTexture(this->texture);
        }
    }
    SDL_Texture *texture;
};

class Timeline {
public:
    int speed = 70;
    Timeline() {
        x = 0.0;
    }
    
    void render() {
        uint8_t r,g,b,a;
        SDL_GetRenderDrawColor(renderer->get(), &r, &g, &b, &a);
        SDL_SetRenderDrawColor(renderer->get(), 255, 255, 0, 255);
        SDL_RenderDrawLine(renderer->get(), x, 0, x, SCREEN_HEIGHT);
        SDL_SetRenderDrawColor(renderer->get(), r, g, b, a);
    }
    
    void move() {
        x += static_cast<double>(speed) / FPS;
        if (x > SCREEN_WIDTH || x < 0) speed = -speed;
    }
    
    void start() { }
    
    SDL_Texture *texture;
    
private:
    double x;
};

int main(int argc, char **argv) {
    try {
        SDL_Initializer sdl_initializer(SDL_INIT_AUDIO);
        WWindow window("Recorder", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        renderer = new WRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED);
        SDL_Event e;
        bool quit = false;
        Grid grid;
        Timeline timerline;
        Timer timer;
        timer.start();
        
        std::vector<double> sin_arr(SCREEN_WIDTH);
        for (int i=0; i <SCREEN_WIDTH;i++) {
            sin_arr[i] = sin(i/30.0) * 300;
        }
        for (auto val : sin_arr) std::cout << val << std::endl;
        
        SDL_Point sin_points[SCREEN_WIDTH];
        for (int i=0; i <SCREEN_WIDTH;i++) {
            sin_points[i] = { i, static_cast<int>(sin_arr[i]) + 300};
        }
        
        while (!quit) {
            while(SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            } 
            
            uint64_t start = timer.get_ticks();
            
            SDL_SetRenderDrawColor(renderer->get(), 0, 0, 0, 255);
            SDL_RenderClear(renderer->get());
            SDL_RenderCopy(renderer->get(), grid.texture, NULL, NULL);
            
            uint8_t r,g,b,a;
            SDL_GetRenderDrawColor(renderer->get(), &r, &g, &b, &a);
            SDL_SetRenderDrawColor(renderer->get(), 255, 255, 0, 255);
            SDL_RenderDrawLines(renderer->get(), sin_points, SCREEN_WIDTH);
            SDL_SetRenderDrawColor(renderer->get(), r, g, b, a);
            
            timerline.render();
            SDL_RenderPresent(renderer->get());
            
            timerline.move();
            
            uint64_t end = timer.get_ticks();
            if (end - start < 1000 / FPS) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000 / FPS - (end-start)));
            }
        }
        
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}