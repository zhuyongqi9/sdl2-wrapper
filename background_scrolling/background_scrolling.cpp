#include <iostream>
#include "SDL_wrapper/wrapper.h"
#include <string>

const std::string PRO_DIR(MACRO_PROJECT_DIR); 

const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;


class ScrollingBackground {
public: 
    ScrollingBackground(WRenderer *renderer): renderer(renderer)
    {
        background  = renderer->create_texture(WPNGSurface(PRO_DIR + "/background_scrolling/bg.png"));
        x = y = 0;
        speed = 10;
    }
    
    void move_up() { y -= speed; if (y < 0) y = 0;}
    void move_down() { y += speed; if ( y > background->height - SCREEN_HEIGHT) y = background->height - SCREEN_HEIGHT;}
    void move_left() { x -= speed; if (x < 0) x = 0;}
    void move_right() { x += speed; if (x > background->width - SCREEN_WIDTH) x = background->width - SCREEN_WIDTH;}
    
    void render() {
        SDL_Rect src = { x, y, SCREEN_WIDTH, SCREEN_HEIGHT};
        background->render(&src, nullptr);
    }
    
private:
    WRenderer *renderer;
    std::unique_ptr<WTexture> background;
    int x;
    int y;
    int speed;
};

ScrollingBackground *background;

class Dot {
public:
    Dot(WRenderer *renderer, double ratio):renderer(renderer) {
        WBMPSurface surface(PRO_DIR + "/background_scrolling/dot.bmp");
        SDL_SetColorKey(surface.get(), SDL_TRUE, SDL_MapRGB(surface.get()->format, 255, 255, 255));
        dot  = renderer->create_texture(&surface);
        radius  = dot->width / 2;
        radius = (int) radius * ratio;
        x = radius;
        y = radius;
        
        speed = 10;
    }
    
    void move_up() {  y -= speed; if ( y < radius ) y = radius; }
    void move_down() {  y += speed; if (y > SCREEN_HEIGHT - radius ) y = SCREEN_HEIGHT - radius;}
    void move_left() {  x -= speed; if (x < radius ) x = radius; }
    void move_right() {  x += speed; if (x > SCREEN_WIDTH - radius ) x = SCREEN_WIDTH - radius; }
    
    void set_speed(int speed) { this->speed = speed; }
    
    bool touch_right_edge() { return x >= SCREEN_WIDTH - radius;}
    bool touch_left_edge() { return x <= radius;}
    bool touch_up_edge() { return y <= radius; }
    bool touch_down_edge() { return y >= SCREEN_HEIGHT - radius; }
    
    void handle_event(SDL_Event &e) {
        if (e.type == SDL_KEYDOWN) {
            switch(e.key.keysym.sym) {
                case SDLK_UP:
                    move_up();
                    if (touch_up_edge()) background->move_up();
                    break;
                case SDLK_DOWN:
                    move_down();
                    if (touch_down_edge()) background->move_down();
                    break;
                case SDLK_LEFT:
                    move_left();
                    if (touch_left_edge()) background->move_left();
                    break;
                case SDLK_RIGHT:
                    move_right();
                    if (touch_right_edge()) background->move_right();
                    break;
            }
        }
    }
    
    void render() {
        SDL_Rect dst = { x-radius, y-radius, radius*2, radius*2};
        dot->render(nullptr, &dst);
    }
private:
    int x;    
    int y;
    int radius;
    int speed;
    WRenderer *renderer;
    std::unique_ptr<WTexture> dot;
};


int main(int argc, char **argv) {
    try {
        SDL_Initializer sdl_initializer(SDL_INIT_AUDIO);
        TTF_Initializer ttf_initializer;
        
        std::unique_ptr<WWindow> window(new WWindow("Scrolling", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
        std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC));
        
        Dot dot(renderer.get(), 3);
        background = new ScrollingBackground(renderer.get());
        
        SDL_Event e;
        bool quit = false;
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }             
                dot.handle_event(e);
            }
            
            renderer->set_color(255, 255, 255, 255);
            renderer->clear();
            background->render();
            dot.render();
            renderer->present();
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}