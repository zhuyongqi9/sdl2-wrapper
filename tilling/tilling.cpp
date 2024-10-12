#include <iostream>
#include "SDL2/SDL.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_keycode.h"
#include "SDL2/SDL_pixels.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_surface.h"
#include "SDL_utils/SDL_wrapper.h"
#include <stdexcept>
#include <vector>
#include <fstream>

const std::string PRO_DIR(MACRO_PROJECT_DIR);

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int LEVEL_WIDHT =  1280;
const int LEVEL_HEIGHT = 960;

const int TILE_WIDTH = 80;
const int TILE_HEIGHT = 80;
const int TOTAL_TILES = 192;
const int TOTAL_TILE_SPRITES = 12;


WRenderer *renderer;
WTexture *tile_texture;

bool checkCollision(SDL_Rect &camera, SDL_Rect &box) {
    return ( (box.x >= camera.x) && (box.x < camera.x + camera.w))
    && ((box.y >= camera.y) && (box.y < camera.y + camera.h));
}

enum {
    TILE_RED,
    TILE_GREEN,
    TIEL_BLUE,
    TILE_CENTER,
    TILE_TOP,
    TILE_TOPRIGHT,
    TILE_RIGHT,
    TILE_BOTTOMRIGHT,
    TILE_BOTTOM,
    TILE_BOTTOMLEFT,
    TILE_LEFT,
    TILE_TOPLEFT,
};

const static std::vector<SDL_Rect> tile_clips = {
    {0, 0, TILE_WIDTH, TILE_HEIGHT},
    {0, 80, TILE_WIDTH, TILE_HEIGHT},
    {0, 160, TILE_WIDTH, TILE_HEIGHT},
    {160, 80, TILE_WIDTH, TILE_HEIGHT},
    {160, 0, TILE_WIDTH, TILE_HEIGHT},
    {240, 0, TILE_WIDTH, TILE_HEIGHT},
    {240, 80, TILE_WIDTH, TILE_HEIGHT},
    {240, 160, TILE_WIDTH, TILE_HEIGHT},
    {160, 160, TILE_WIDTH, TILE_HEIGHT},
    {80, 160, TILE_WIDTH, TILE_HEIGHT},
    {80, 80, TILE_WIDTH, TILE_HEIGHT},
    {80, 0, TILE_WIDTH, TILE_HEIGHT},
};

class Tile {
public:
    Tile(int x, int y, int tile_type): type(tile_type) 
    {
        box.x = x;
        box.y = y;
        box.w = TILE_WIDTH;
        box.h = TILE_HEIGHT;
    }
    
    void render(SDL_Rect camera) {
        if (checkCollision(camera, box)) {
            SDL_Rect dst = { box.x - camera.x, box.y - camera.y, TILE_WIDTH, TILE_HEIGHT };
            SDL_RenderCopy(renderer->get(), tile_texture->get(), &tile_clips[type], &dst);
        }
    }
    
    int get_type() {return type;}
    SDL_Rect get_box() {return box;};
private:
    SDL_Rect box;
    int type;    
};

bool touches_wall(SDL_Rect box, Tile* tiles[]) {
    for (int i = 0; i < TOTAL_TILES; i++) {
        if ((tiles[i]->get_type() >= TILE_CENTER) && (tiles[i]->get_type() <= TILE_TOPLEFT)) {
            SDL_Rect tile_box = tiles[i]->get_box();
            if (checkCollision(tile_box, box)) {
                return true;
            }
        }
    } 
    return false;
}

class Dot {
public:   
    static const int DOT_WIDTH = 20;
    static const int DOT_HEIGHT = 20;
    static const int DOT_VEL = 10;
    
    Dot(): vel_x(DOT_VEL), vel_y(DOT_VEL)
    {
        WPNGSurface surface(PRO_DIR + "/tilling/dot.bmp");
        SDL_SetColorKey(surface.get(), SDL_TRUE,SDL_MapRGB(surface.get()->format, 0, 255, 255));
        texture = new WTexture(renderer->get(), surface.get());
        box = {0, 0, texture->width, texture->height};
    }
    
    void render(SDL_Rect &camera) {
        SDL_Rect dst = { box.x - camera.x, box.y - camera.y, texture->width, texture->height};
        SDL_RenderCopy(renderer->get(), texture->get(), NULL, &dst);
    }
    
    void moveLeft(Tile *tiles[]) {
        box.x -= vel_x;
        if (box.x < 0 || touches_wall(box, tiles)) {
            box.x += vel_x;
        }
    }
    
    void moveRight(Tile *tiles[]) {
        box.x += vel_x;
        if (((box.x + DOT_WIDTH) > LEVEL_WIDHT) || touches_wall(box, tiles)) {
            box.x -= vel_x;
        }
    }
    
    void moveUp(Tile *tiles[]) {
        box.y -= vel_y;
        if (box.y < 0 || touches_wall(box, tiles)) {
            box.y += vel_y;
        }
    }
    
    void moveDown(Tile *tiles[]) {
        box.y += vel_y;
        if (((box.y + DOT_HEIGHT) > LEVEL_HEIGHT) || touches_wall(box, tiles)) {
            box.y -= vel_y;
        }
    }
    
    void setCamera(SDL_Rect &camera) {
        int camera_x = (box.x + DOT_WIDTH / 2) - SCREEN_WIDTH / 2;
        int camera_y = (box.y + DOT_HEIGHT/ 2) - SCREEN_HEIGHT/2;
        
        if (camera_x < 0) {
            camera_x = 0;
        } else if (camera_x > LEVEL_WIDHT - camera.w) {
            camera_x = LEVEL_WIDHT - camera.w;
        }
        
        if (camera_y < 0) {
            camera_y = 0;
        } else if (camera_y > LEVEL_HEIGHT - camera.h) {
            camera_y = LEVEL_HEIGHT - camera.h;
        }
        
        camera.x = camera_x;
        camera.y = camera_y;
        camera.w = SCREEN_WIDTH;
        camera.h = SCREEN_HEIGHT;
    }
private:
    WTexture *texture;
    SDL_Rect box;
    int vel_x, vel_y;
};

Tile* tiles[TOTAL_TILES];
void load_map() {
    std::ifstream map(PRO_DIR + "/tilling/lazy.map");
    if (map.fail()) {
        throw std::runtime_error("failed to load map");
    }
    
    int x,y;
    x = y = 0;
    for (int i = 0; i < TOTAL_TILES; i++) {
        int tile_type = -1;
        map >> tile_type;
        if (map.fail()) {
            throw std::runtime_error("failed to read map, unexpectd EOF");
        }
        
        if ((tile_type >= 0) && (tile_type < TOTAL_TILE_SPRITES)) {
            tiles[i] = new Tile(x, y, tile_type);
        } else {
            throw std::runtime_error("unknow map type: " + std::to_string(tile_type));
        }
        x += TILE_WIDTH;
        if (x >= LEVEL_WIDHT) {
            x = 0;
            y += TILE_HEIGHT;
        }
    }
    
}

int main(int argc, char **argv) {
    try {
        SDL_Initializer sdl_initializer(SDL_INIT_VIDEO);
        IMG_Initializer img_initialzer(IMG_INIT_PNG);
        
        WWindow window("Tilling", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        renderer = new WRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED);
        
        WPNGSurface surface_tile(PRO_DIR + "/tilling/tiles.png");
        tile_texture = new WTexture(renderer->get(), surface_tile.get());
        
        load_map();
        
        SDL_Event e;
        bool quit = false;
        Dot dot;
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                } else if (e.type == SDL_KEYDOWN) {
                    switch (e.key.keysym.sym) {
                        case SDLK_DOWN:
                            dot.moveDown(tiles);
                            break;
                        case SDLK_UP:
                            dot.moveUp(tiles);
                            break;
                        case SDLK_LEFT:
                            dot.moveLeft(tiles);
                            break;
                        case SDLK_RIGHT:
                            dot.moveRight(tiles);
                            break;
                    }
                }
            }
            
            SDL_Rect camera;
            dot.setCamera(camera);
            
            SDL_SetRenderDrawColor(renderer->get(), 0, 0, 0, 255);
            SDL_RenderClear(renderer->get());
            for (auto item : tiles) {
                item->render(camera);
            }
            dot.render(camera);
            SDL_RenderPresent(renderer->get());
        }
    } catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}