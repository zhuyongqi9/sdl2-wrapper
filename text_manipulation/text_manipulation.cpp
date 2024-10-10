#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <exception>
#include <iostream>
#include "SDL_utils/SDL_wrapper.h"

const std::string PRO_DIR(MACRO_PROJECT_DIR);

const int SCALE = 1;
const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

int main(int argc, char **argv) {
    try {
        SDL_Initializer sdl_initializer(SDL_INIT_VIDEO);
        IMG_Initializer img_intializer(IMG_INIT_PNG);
        WWindow window("Texture Manipulation", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        WRenderer renderer(window.get(), -1, SDL_RENDERER_ACCELERATED);
        WPNGSurface surface_png(PRO_DIR + "/text_manipulation/foo.png");
        SDL_Surface *pixel_surface = SDL_ConvertSurfaceFormat(surface_png.get(), SDL_GetWindowPixelFormat(window.get()), 0);
        
        SDL_SetColorKey(surface_png.get(), SDL_TRUE, SDL_MapRGB(surface_png.get()->format, 255, 0,  255));
        WTexture texture(renderer.get(), surface_png.get());
        
        SDL_Event e;
        bool quit = false;
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }
            
            SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 255);
            SDL_RenderClear(renderer.get());
            SDL_Rect dst = { 0, 0, texture.width, texture.height };
            SDL_RenderCopy(renderer.get(), texture.get(), NULL, &dst);
            SDL_RenderPresent(renderer.get());
        }
        
    }catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}