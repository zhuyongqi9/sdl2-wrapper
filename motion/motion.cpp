#include <csignal>
#include <iostream>
#include <thread>
#include "SDL_utils/SDL_wrapper.h"
#include "utils.h"

const std::string PRO_DIR(MACRO_PROJECT_DIR);

const int SCALE = 2;
const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

class Point {
public:
    Point(SDL_Renderer *render, std::string path);
    ~Point();
    
    void move_up() {  y -= speed; if ( y < radius ) y = radius; }
    void move_down() {  y += speed; if (y > SCREEN_HEIGHT - radius ) y = SCREEN_HEIGHT - radius;}
    void move_left() {  x -= speed; if (x < radius ) x = radius; }
    void move_right() {  x += speed; if (x > SCREEN_WIDTH - radius ) x = SCREEN_WIDTH - radius; }
    
    void set_radius(int raidus) { this->radius = raidus; }
    void set_radius_radio(double ratio) { this->radius = (int) radius * ratio; }
    void set_speed(int speed) { this->speed = speed; }
    
    void render();
private:
    int x;    
    int y;
    int radius;
    int speed;
    SDL_Renderer *renderer;
    WTexture *texture;
};

Point::Point(SDL_Renderer *renderer, std::string path): renderer(renderer)
{
    WBMPSurface surface(path);
    texture = new WTexture(renderer, surface.get());
    radius  = texture->width / 2;
    x = radius;
    y = radius;
    speed = 10;
}

Point::~Point() 
{
    if (texture == nullptr) {
        delete texture;
    }
}

void Point::render() {
    SDL_Rect dst = { x-radius, y-radius, radius*2, radius*2};
    SDL_RenderCopy(renderer, texture->get(), NULL, &dst);
}

int main(int argc, char **argv) {
    try {
        SDL_Initializer sdl_initializer(SDL_INIT_VIDEO);
        WWindow window("Motion", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        WRenderer renderer(window.get(), -1, SDL_RENDERER_ACCELERATED);
        
        bool quit = false;
        SDL_Event e;
        Point point(renderer.get(), PRO_DIR + "/motion/dot.bmp");
        point.set_radius_radio(2);
        
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                } else if (e.type == SDL_KEYDOWN) {
                    switch(e.key.keysym.sym) {
                        case SDLK_UP:
                            point.move_up();
                            break;
                        case SDLK_DOWN:
                            point.move_down();
                            break;
                        case SDLK_LEFT:
                            point.move_left();
                            break;
                        case SDLK_RIGHT:
                            point.move_right();
                            break;
                        default:
                            break;
                    }
                }
            }
            
            SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 255);
            SDL_RenderClear(renderer.get());
            point.render();
            SDL_RenderPresent(renderer.get());
        }
        
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}