
#include <SDL_wrapper/wrapper.h>
#include <iostream>
#include <vector>

const std::string PRO_DIR(MACRO_PROJECT_DIR);

const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

class PlayingButton {
public:
    PlayingButton(WRenderer *renderer, SDL_Point dst):renderer(renderer),dst(dst) {
        status = stopped;
        region = { dst.x, dst.y, width, height};
        
        WPNGSurface playing_surface(PRO_DIR + "/widgets/playing_button/playing_button_playing.png");
        playing_surface.set_color_key(0, 0, 0);
        playing_icon.reset(renderer->create_texture(&playing_surface));
        
        WPNGSurface paused_surface(PRO_DIR + "/widgets/playing_button/playing_button_paused.png");
        paused_surface.set_color_key(0, 0, 0);
        paused_icon.reset(renderer->create_texture(&paused_surface));
    }
    
    void handle_event(SDL_Event &e) {
        SDL_Point point;
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            SDL_GetMouseState(&point.x, &point.y);
            if (in_region(point, region))  {
                if (status == stopped) {
                    std::cout << "playing" << std::endl;
                    status = playing;
                } else if (status == playing) {
                    std::cout << "paused" << std::endl;
                    status = paused;
                } else if (status == paused) {
                    std::cout << "replaying" << std::endl;
                    status = playing;
                }
            }
        } 
    };
    
    void render() {
        if (status == stopped) {
            playing_icon->render(nullptr, &region);
        } else if (status == playing) {
            paused_icon->render(nullptr, &region);
        } else if (status == paused) {
            playing_icon->render(nullptr, &region);
        }
    }
private:
    enum state {
        stopped,
        playing,
        paused,
    };
    
    WRenderer *renderer;
    
    SDL_Point dst;
    SDL_Rect region;
    int width = 108;
    int height = 108;
    
    state status;
    
    std::unique_ptr<WTexture> playing_icon;
    std::unique_ptr<WTexture> paused_icon;
    
    bool in_region(SDL_Point &point, SDL_Rect &region) {
        return (point.x >= region.x && point.x <= region.x + region.w)
        && (point.y >= region.y && point.y <= region.y + region.h);
    }
};

int main() {
    
    SDL_Initializer sdl_initializer(SDL_INIT_AUDIO);
    TTF_Initializer ttf_initializer;
    
    std::unique_ptr<WWindow> window(new WWindow("Playing Button", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
    std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED));
    
    bool quit = false;
    SDL_Event e;
    std::vector<std::string> list_items(0); 
    for (int i = 0; i < 10; i++) list_items.push_back("device " + std::to_string(i));
    PlayingButton button(renderer.get(), {20, 20});
    
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            button.handle_event(e);
        }
        
        renderer->set_color(82, 82, 82, 255);
        renderer->clear();
        button.render();
        renderer->present();
    }
}