#include <iostream>
#include <SDL_wrapper/wrapper.h>
#include <string>

const std::string PRO_DIR(MACRO_PROJECT_DIR); 

const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

const int SPEED = 10;

class InfiniteScrollingBackground {
public: 
    InfiniteScrollingBackground(WRenderer *renderer): renderer(renderer)
    {
        picture  = renderer->create_texture(WPNGSurface(PRO_DIR + "/background_infinite/bg.png"));
        x = y = 0;
        speed = SPEED;
        offset = 0;
    }
    
    void move_up() { y -= speed; if (y < 0) y = 0;}
    void move_down() { y += speed; if ( y > picture->height - SCREEN_HEIGHT) y = picture->height - SCREEN_HEIGHT;}
    void move_left() { x -= speed; if (x < 0) x = 0;}
    void move_right() { 
        
        if (offset > 0) {
            offset -= speed;
        } else {
            x += SPEED;
            if (x >= picture->width - SCREEN_WIDTH) {
                offset = SCREEN_WIDTH;
                x = 0;
            }
        }
    }
    
    void render() {
        if (offset > 0) {
            SDL_Rect src1 = {picture->width - offset, y, offset, SCREEN_HEIGHT};
            SDL_Rect dst1 = {0, 0, offset, SCREEN_HEIGHT};
            picture->render(&src1, &dst1);
            
            SDL_Rect src2 = {x, y, SCREEN_WIDTH - offset, SCREEN_HEIGHT};
            SDL_Rect dst2 = {offset, 0, SCREEN_WIDTH-offset, SCREEN_HEIGHT};
            picture->render(&src2, &dst2);
        } else {
            SDL_Rect src = { x, y, SCREEN_WIDTH, SCREEN_HEIGHT};
            picture->render(&src, nullptr);
        }
    }
private:
    WRenderer *renderer;
    std::unique_ptr<WTexture> picture;
    int x;
    int y;
    int speed;
    int offset;
};

InfiniteScrollingBackground *background;

class Dot {
public:
    Dot(WRenderer *renderer, double ratio):renderer(renderer) {
        WBMPSurface surface(PRO_DIR + "/background_infinite/dot.bmp");
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
        SDL_Initializer sdl_initializer(SDL_INIT_VIDEO);
        IMG_Initializer img_initializer(IMG_INIT_PNG);
        
        std::unique_ptr<WWindow> window(new WWindow("Infinite Background", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
        std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC));
        
        Dot dot(renderer.get(), 3);
        background = new InfiniteScrollingBackground(renderer.get());
        
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
