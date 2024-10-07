#include <SDL2/SDL_events.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <stdexcept>
#include "SDL_utils/SDL_wrapper.h"

const std::string PRO_DIR(MACRO_PROJECT_DIR);
const int SCALE = 2;
const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

int main(int argc, char **argv) {
    try {
        SDL_Initializer initializer(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
        IMG_Initializer img_initializer(IMG_INIT_PNG);
        Mix_Initializer mix_initializer;
        
        WWindow window("Mouse Event", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        WRenderer renderer(window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        
        WPNGSurface surface(PRO_DIR + "/sound_effects_and_music/prompt.png"); 
        WTexture texture_default(renderer.get(), surface.get());

        WMUS music(PRO_DIR+ "/sound_effects_and_music/beat.wav");
        WWAV scratch(PRO_DIR+ "/sound_effects_and_music/scratch.wav");
        WWAV high(PRO_DIR+ "/sound_effects_and_music/high.wav");
        WWAV medium(PRO_DIR+ "/sound_effects_and_music/medium.wav");
        WWAV low(PRO_DIR+ "/sound_effects_and_music/low.wav");
        
        SDL_Event e;
        bool quit = false;
        while(!quit) {
            while(SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                } else if (e.type == SDL_KEYDOWN) {
                    switch (e.key.keysym.sym) {
                        case SDLK_1:
                            Mix_PlayChannel(-1, high.get(), 0);
                            break;
                        case SDLK_2:
                            Mix_PlayChannel(-1, medium.get(), 0);
                            break;
                        case SDLK_3:
                            Mix_PlayChannel(-1, low.get(), 0);
                            break;
                        case SDLK_4:
                            Mix_PlayChannel(-1, scratch.get(), 0);
                            break;
                        case SDLK_9:
                            if (Mix_PlayingMusic() == 0) {
                                Mix_PlayMusic(music.get(), -1);
                            } else {
                                if (Mix_PausedMusic() == 1) {
                                    Mix_ResumeMusic();
                                } else {
                                    Mix_PauseMusic();
                                }
                            }
                            break;
                        case SDLK_0:
                            Mix_HaltMusic();
                            break;
                    }
                }
            }
            
            
            SDL_SetRenderDrawColor(renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(renderer.get());
            
            SDL_RenderCopy(renderer.get(), texture_default.get(), NULL, NULL);
            SDL_RenderPresent(renderer.get());
        } 

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}