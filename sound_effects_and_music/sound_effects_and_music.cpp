#include <iostream>
#include <stdexcept>
#include <SDL_wrapper/wrapper.h>

const std::string PRO_DIR(MACRO_PROJECT_DIR);

const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

int main(int argc, char **argv) {
    try {
        SDL_Initializer sdl_initializer(SDL_INIT_AUDIO);
        Mix_Initializer mix_initializer;
        
        std::unique_ptr<WWindow> window(new WWindow("Sound Effects & Music", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
        std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED));
        
        std::shared_ptr<WTexture> picture(renderer->create_texture(WPNGSurface(PRO_DIR + "/sound_effects_and_music/prompt.png")));

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
                            high.play_channel(-1, 0);
                            break;
                        case SDLK_2:
                            low.play_channel(-1, 0);
                            break;
                        case SDLK_3:
                            medium.play_channel(-1, 0);
                            break;
                        case SDLK_4:
                            scratch.play_channel(-1, 0);
                            break;
                        case SDLK_9:
                            if (Mix_PlayingMusic() == 0) {
                                music.play(-1);
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
            
            
            renderer->set_color(0xFF, 0xFF, 0xFF, 0xFF);
            renderer->clear();
            picture->render(nullptr, nullptr);
            renderer->present();
        } 

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}