
#include <SDL2/SDL_events.h>
#include <pthread.h>

#include <iostream>
#include <SDL_wrapper/wrapper.h>
#include <string>
#include <vector>

const std::string PRO_DIR(MACRO_PROJECT_DIR);

const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;


class Grid {
public: 
    Grid(WRenderer *renderer):renderer(renderer), picked(y_cnt, std::vector<bool>(x_cnt, false)){
        status = finished;
    } 
    
    void handle_event(SDL_Event &e) {
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            if (status == finished) {
                status = picking;
                for (int i = 0; i < y_cnt; i++) {
                    for (int j = 0; j < x_cnt; j++) {
                        picked[i][j] = false; 
                    }
                }
            }
        } else if (e.type == SDL_MOUSEBUTTONUP) {
            if (status == picking) {
                status = finished;
            }
        } else if (e.type == SDL_MOUSEMOTION) {
            if (status == picking) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                picked[y/GRID_SIZE][x/GRID_SIZE] = true;
            }
        }
    }
    
    void render() {
        // draw pickked grid
        renderer->set_color(255, 255, 0, 255);
        for (int i = 0; i < y_cnt; i++) {
            for (int j = 0; j < x_cnt; j++) {
                if (picked[i][j]) {
                    SDL_Rect dst = {j * GRID_SIZE, i*GRID_SIZE, GRID_SIZE, GRID_SIZE};
                    renderer->fill_rect(&dst);
                }
            }
        }
        
        // draw grid
        renderer->set_color(80, 80, 80, 255);
        for (int i = 0; i < x_cnt; i++) {
            renderer->draw_line({i * GRID_SIZE, 0}, {i * GRID_SIZE, height});
        }
        for (int i = 0; i < y_cnt; i++) {
            renderer->draw_line({0, i * GRID_SIZE}, {width, i * GRID_SIZE});
        }
    }
    
private:
    WRenderer *renderer;
    int GRID_SIZE = 20;
    int x_cnt = SCREEN_WIDTH / GRID_SIZE;
    int y_cnt = SCREEN_HEIGHT / GRID_SIZE;
    std::vector<std::vector<bool>> picked;
    
    int width = SCREEN_WIDTH;
    int height  =  SCREEN_HEIGHT;
    
    enum state{
        picking,
        finished,
    };
    state status;
};

int main(int argc, char **argv) {
    SDL_Initializer sdl_initializer(SDL_INIT_AUDIO);
    IMG_Initializer img_initializer(IMG_INIT_PNG);
    
    std::unique_ptr<WWindow> window(new WWindow("Grid", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
    std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED));
    std::shared_ptr<WTexture> picture(renderer->create_texture(WPNGSurface(PRO_DIR + "/texture_png/images.png")));
    
    Grid grid(renderer.get());
    
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }         
            grid.handle_event(e);
        }

        renderer->set_color(0, 0, 0, 255);
        renderer->clear();
        grid.render();
        renderer->present();
    }
    return 0;
}