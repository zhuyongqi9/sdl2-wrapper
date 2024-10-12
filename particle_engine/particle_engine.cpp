#include <cstdlib>
#include <iostream>
#include <thread>
#include "SDL2/SDL.h"
#include "SDL2/SDL_pixels.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_stdinc.h"
#include "SDL2/SDL_surface.h"
#include "SDL2/SDL_video.h"
#include "SDL_utils/SDL_wrapper.h"
#include "utils.h"

const int TICKS_PRE_FRAME = 1000/60;

const std::string PRO_DIR(MACRO_PROJECT_DIR);

const int SCALE = 1;
const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;
const int TOTAL_PARTICLES = 20;

WTexture *red;
WTexture *blue;
WTexture *green;
WTexture *dot;
WTexture *shimmer;

WRenderer *renderer;

class Particle {
public:
    Particle(int x, int y) {
        pos_x = x - 5 + ( rand() % 25);
        pos_y = y - 5 + ( rand() & 25);
        
        frame = rand() % 25;
        switch (rand() % 3) {
            case 0:
                texture = red;
                break;
            case 1:
                texture = blue;
                break;
            case 2:
                texture = green;
                break;
        }
    }
    void render() {
        {
            SDL_Rect dst = { pos_x, pos_y, texture->width, texture->height };
            SDL_RenderCopy(renderer->get(), texture->get(), NULL, &dst);
        }
        if (frame % 2 == 0) {
            SDL_Rect dst = { pos_x, pos_y, shimmer->width, shimmer->height};
            SDL_RenderCopy(renderer->get(), shimmer->get(), NULL, &dst);
        }
        frame++;
    }
    bool isDead() {return frame > 50;}
        
private:
    int pos_x;
    int pos_y; 
    int frame;
    WTexture *texture;
};

class Dot {
public:
    const int DOT_WIDTH = 20; 
    const int DOT_HEIGHT = 20;
    
    const int DOT_VEL = 10;
    
    Dot() {
        pos_x = 0;
        pos_y = 0;
        vel_x = 0;
        vel_y = 0;
        
        for (auto &item :particles) {
            item = new Particle(pos_x, pos_y);
        }
    }
    
    ~Dot() {
        for (int i = 0; i < TOTAL_PARTICLES; i++) {
            delete particles[i];
        }
    }
    
    void handleEvent(SDL_Event &e) {
    }
    
    void move() {
        pos_x += 1;
        pos_y += 1;
    }
    
    void render() {
        SDL_Rect dst = { pos_x, pos_y, dot->width, dot->height};
        SDL_RenderCopy(renderer->get(), dot->get(), NULL, &dst);
        render_particles();
    }
    
private:
    int pos_x;
    int pos_y;
    Particle* particles[TOTAL_PARTICLES];
    void render_particles() {
        for (auto &item:particles) {
            if (item->isDead()) item = new Particle(pos_x, pos_y);
            item->render();
        }
    }
    int vel_x, vel_y;
};

int main(int argc, char **argv) {
    SDL_Initializer sdl_initializer(SDL_INIT_VIDEO);
    WWindow window("Particle Engine", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = new WRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED);
    
    WBMPSurface surface_blue(PRO_DIR + "/particle_engine/blue.bmp");
    WBMPSurface surface_red(PRO_DIR + "/particle_engine/red.bmp");
    WBMPSurface surface_green(PRO_DIR + "/particle_engine/green.bmp");
    WBMPSurface surface_dot(PRO_DIR + "/particle_engine/dot.bmp");
    WBMPSurface surface_shimmer(PRO_DIR + "/particle_engine/shimmer.bmp");
    SDL_SetColorKey(surface_blue.get(), SDL_TRUE, SDL_MapRGB(surface_blue.get()->format, 0, 255, 255));
    SDL_SetColorKey(surface_red.get(), SDL_TRUE, SDL_MapRGB(surface_red.get()->format, 0, 255, 255));
    SDL_SetColorKey(surface_green.get(), SDL_TRUE, SDL_MapRGB(surface_green.get()->format, 0, 255, 255));
    SDL_SetColorKey(surface_dot.get(), SDL_TRUE, SDL_MapRGB(surface_dot.get()->format, 0, 255, 255));
    SDL_SetColorKey(surface_shimmer.get(), SDL_TRUE, SDL_MapRGB(surface_shimmer.get()->format, 0, 255, 255));
    
    blue = new WTexture(renderer->get(), surface_blue.get());
    red = new WTexture(renderer->get(), surface_red.get());
    green = new WTexture(renderer->get(), surface_green.get());
    dot = new WTexture(renderer->get(), surface_dot.get());
    shimmer = new WTexture(renderer->get(), surface_shimmer.get());
    
    SDL_SetTextureAlphaMod(blue->get(), 192);
    SDL_SetTextureAlphaMod(red->get(), 192);
    SDL_SetTextureAlphaMod(green->get(), 192);
    SDL_SetTextureAlphaMod(shimmer->get(), 192);
    
    SDL_Event e;
    bool quit = false;
    Dot dot;
    double rate;
    uint64_t total_frames;
    uint64_t ticks_count;
    Timer timer;
    timer.start();
    
    while(!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
        uint64_t start = timer.get_ticks();
        rate = total_frames / (timer.get_ticks() / 1000.0);
        
        SDL_SetRenderDrawColor(renderer->get(), 255, 255, 255, 255);
        SDL_RenderClear(renderer->get());
        dot.move();
        dot.render();
        SDL_RenderPresent(renderer->get());
        
        total_frames++;
        uint64_t end = timer.get_ticks();
        if ((end - start) < TICKS_PRE_FRAME) {
            std::this_thread::sleep_for(std::chrono::milliseconds(TICKS_PRE_FRAME - (end-start)));
        }
    }
}
