#include <SDL2/SDL_pixels.h>
#include <iostream>
#include "SDL_utils/SDL_wrapper.h"
#include <string>

const std::string PRO_DIR(MACRO_PROJECT_DIR); 

const int SCALE = 1;
const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

const int SPEED = 10;
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
    
    bool right_edge() { return x == SCREEN_WIDTH - radius;}
    bool left_edge() { return x == radius;}
    bool up_edge() { return y == radius; }
    bool down_edge() { return y == SCREEN_HEIGHT - radius; }
    
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
    SDL_SetColorKey(surface.get(), SDL_TRUE, SDL_MapRGB(surface.get()->format, 255, 255, 255));
    texture = new WTexture(renderer, surface.get());
    radius  = texture->width / 2;
    x = radius;
    y = radius;
    speed = SPEED;
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

class ScrollingBackground {
public: 
    ScrollingBackground(SDL_Renderer *renderer, std::string path): renderer(renderer)
    {
        WPNGSurface surface_back(path);
        texture = new WTexture(renderer, surface_back.get());
        x = y = 0;
        speed = SPEED;
    }
    
    void move_up() { y -= speed; if (y < 0) y = 0;}
    void move_down() { y += speed; if ( y > texture->height - SCREEN_HEIGHT) y = texture->height - SCREEN_HEIGHT;}
    void move_left() { x -= speed; if (x < 0) x = 0;}
    void move_right() { x += speed; if (x > texture->width - SCREEN_WIDTH) x = texture->width - SCREEN_WIDTH;}
    
    void render() {
        SDL_Rect src = { x, y, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, texture->get(), &src, NULL);
    }
private:
    SDL_Renderer *renderer;
    WTexture *texture;
    int x;
    int y;
    int speed;
};

int main(int argc, char **argv) {
    try {
        SDL_Initializer sdl_initializer(SDL_INIT_VIDEO);
        IMG_Initializer img_initializer(IMG_INIT_PNG);
        WWindow window("Scrolling", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        WRenderer renderer(window.get(), -1, SDL_RENDERER_ACCELERATED);
        
        ScrollingBackground background(renderer.get(), PRO_DIR + "/scrolling/bg.png");
        
        Point point(renderer.get(), PRO_DIR + "/scrolling/dot.bmp");
        point.set_radius_radio(3);
        int x = 0, y = 0;
        
        SDL_Event e;
        bool quit = false;
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                } else if (e.type == SDL_KEYDOWN) {
                    switch (e.key.keysym.sym) {
                        case SDLK_UP:
                            point.move_up();
                            if (point.up_edge()) background.move_up();
                            break;
                        case SDLK_DOWN:
                            point.move_down();
                            if (point.down_edge()) background.move_down();
                            break;
                        case SDLK_LEFT:
                            point.move_left();
                            if (point.left_edge()) background.move_left();
                            break;
                        case SDLK_RIGHT:
                            point.move_right();
                            if (point.right_edge()) background.move_right();
                            break;
                        default:
                            break;
                    }
                }
            }
            
            SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 255);
            SDL_RenderClear(renderer.get());
            background.render();
            point.render();
            SDL_RenderPresent(renderer.get());
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}