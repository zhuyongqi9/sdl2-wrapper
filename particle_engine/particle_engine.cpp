#include <cstdlib>
#include <iostream>
#include <thread>
#include "SDL2/SDL.h"
#include "SDL2/SDL_pixels.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_stdinc.h"
#include "SDL2/SDL_surface.h"
#include "SDL2/SDL_video.h"
#include <SDL_wrapper/wrapper.h>
#include "utils.h"

const int TICKS_PRE_FRAME = 1000/60;

const std::string PRO_DIR(MACRO_PROJECT_DIR);

const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;
const int TOTAL_PARTICLES = 20;

WTexture *g_red;
WTexture *g_blue;
WTexture *g_green;
WTexture *g_dot;
WTexture *g_shimmer;

class Particle {
public:
    Particle(WRenderer *renderer, int x, int y):renderer(renderer) {
        pos_x = x - 5 + ( rand() % 25);
        pos_y = y - 5 + ( rand() & 25);
        
        frame = rand() % 25;
        switch (rand() % 3) {
            case 0:
                texture = g_red;
                break;
            case 1:
                texture = g_blue;
                break;
            case 2:
                texture = g_green;
                break;
        }
    }
    void render() {
        {
            SDL_Rect dst = { pos_x, pos_y, texture->width, texture->height };
            SDL_RenderCopy(renderer->get(), texture->get(), NULL, &dst);
        }
        if (frame % 2 == 0) {
            SDL_Rect dst = { pos_x, pos_y, g_shimmer->width, g_shimmer->height};
            SDL_RenderCopy(renderer->get(), g_shimmer->get(), NULL, &dst);
        }
        frame++;
    }
    bool isDead() {return frame > 50;}
        
private:
    WRenderer *renderer;
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
    
    Dot(WRenderer *renderer): renderer(renderer) {
        pos_x = 0;
        pos_y = 0;
        vel_x = 0;
        vel_y = 0;
        
        for (auto &item :particles) {
            item = new Particle(renderer,pos_x, pos_y);
        }
        
        WBMPSurface surface_dot(PRO_DIR + "/particle_engine/dot.bmp");
        surface_dot.set_color_key( 0, 255, 255);
        dot  = renderer->create_texture(&surface_dot);
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
        dot->render(nullptr, &dst);
        render_particles();
    }
    
private:
    int pos_x;
    int pos_y;
    Particle* particles[TOTAL_PARTICLES];
    WRenderer *renderer;
    
    std::unique_ptr<WTexture> dot;
    
    void render_particles() {
        for (auto &item:particles) {
            if (item->isDead()) item = new Particle(renderer,pos_x, pos_y);
            item->render();
        }
    }
    int vel_x, vel_y;
};

int main(int argc, char **argv) {
    SDL_Initializer sdl_initializer(SDL_INIT_VIDEO);
    std::unique_ptr<WWindow> window(new WWindow("Particle Engine", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE |SDL_WINDOW_SHOWN));
    std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED));
    
    WBMPSurface surface_blue(PRO_DIR + "/particle_engine/blue.bmp");
    WBMPSurface surface_red(PRO_DIR + "/particle_engine/red.bmp");
    WBMPSurface surface_green(PRO_DIR + "/particle_engine/green.bmp");
    WBMPSurface surface_shimmer(PRO_DIR + "/particle_engine/shimmer.bmp");
    
    surface_blue.set_color_key( 0, 255, 255);
    surface_red.set_color_key( 0, 255, 255);
    surface_green.set_color_key( 0, 255, 255);
    surface_shimmer.set_color_key( 0, 255, 255);
    
    std::unique_ptr<WTexture> blue(renderer->create_texture(&surface_blue));
    std::unique_ptr<WTexture> red(renderer->create_texture(&surface_red));
    std::unique_ptr<WTexture> green(renderer->create_texture(&surface_green));
    std::unique_ptr<WTexture> shimmer(renderer->create_texture(&surface_shimmer));
    
    g_blue = blue.get();
    g_red = red.get();
    g_green = green.get();
    g_shimmer = shimmer.get();
    
    SDL_SetTextureAlphaMod(blue->get(), 192);
    SDL_SetTextureAlphaMod(red->get(), 192);
    SDL_SetTextureAlphaMod(green->get(), 192);
    SDL_SetTextureAlphaMod(shimmer->get(), 192);
    
    SDL_Event e;
    bool quit = false;
    Dot dot(renderer.get());
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
        
        renderer->set_color(255, 255, 255, 255);
        renderer->clear();
        dot.move();
        dot.render();
        renderer->present();
        
        total_frames++;
        uint64_t end = timer.get_ticks();
        if ((end - start) < TICKS_PRE_FRAME) {
            std::this_thread::sleep_for(std::chrono::milliseconds(TICKS_PRE_FRAME - (end-start)));
        }
    }
}
