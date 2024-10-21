#include <SDL2/SDL_mouse.h>
#include <SDL_wrapper/wrapper.h>
#include <iostream>
#include <vector>

const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

const std::string PRO_DIR(MACRO_PROJECT_DIR);

class RecordingButton {
public: 
    RecordingButton(WRenderer *renderer, SDL_Point dst): renderer(renderer), dst(dst) {
        status = stopped;
        
        WPNGSurface stopped_icon_surface(WPNGSurface(PRO_DIR + "/widgets/recording_button/recording_button.png"));
        stopped_icon_surface.set_color_key(0, 0, 0);
        stopped_icon.reset(renderer->create_texture(&stopped_icon_surface));
        
        WPNGSurface recording_icon_surface(WPNGSurface(PRO_DIR + "/widgets/recording_button/recording_button_recording.png"));
        recording_icon_surface.set_color_key(0, 0, 0);
        recording_icon.reset(renderer->create_texture(&recording_icon_surface));
        
        region = { dst.x, dst.y, width, height};
    }
    
    void handle_event(SDL_Event &e) {
        SDL_Point point;
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            SDL_GetMouseState(&point.x, &point.y);
            if (in_region(point, region))  {
                if (status == stopped) {
                    std::cout << "playing" << std::endl;
                    status = recording;
                } else if (status == recording) {
                    status = stopped;
                }
            }
        } 
    }
    
    void render() {
        SDL_Rect dst_rect = { dst.x , dst.y, width, height};
        if (status == stopped) {
            stopped_icon->render(nullptr, &dst_rect);
        } else if (status == recording) {
            recording_icon->render(nullptr, &dst_rect);
        }
    }
private:
    enum state {
        recording,
        paused,
        stopped
    };
    WRenderer *renderer; 
    std::unique_ptr<WTexture> stopped_icon;
    std::unique_ptr<WTexture> recording_icon;
    SDL_Point dst;
    SDL_Rect region;
    
    state status;
    
    //1080 * 658
    int width = 108;
    int height = 108;
    
    bool in_region(SDL_Point &point, SDL_Rect &region) {
        return (point.x >= region.x && point.x <= region.x + region.w)
        && (point.y >= region.y && point.y <= region.y + region.h);
    }
};

int main() {
    
    SDL_Initializer sdl_initializer(SDL_INIT_AUDIO);
    TTF_Initializer ttf_initializer;
    
    std::unique_ptr<WWindow> window(new WWindow("Selected List", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
    std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED));
    
    bool quit = false;
    SDL_Event e;
    std::vector<std::string> list_items(0); 
    for (int i = 0; i < 10; i++) list_items.push_back("device " + std::to_string(i));
    RecordingButton button(renderer.get(), {20, 20});
    
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