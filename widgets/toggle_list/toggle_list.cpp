#include <SDL_wrapper/wrapper.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const std::string PRO_DIR(MACRO_PROJECT_DIR);

class ToggleList {
public:
    void handle_event() {
    }
    
    void render(SDL_Rect &&dst) {
        
    }
private:
    
};

int main() {
    SDL_Initializer sdl_initializer(SDL_INIT_AUDIO);
    IMG_Initializer img_initializer(IMG_INIT_PNG);
    
    std::unique_ptr<WWindow> window(new WWindow("Toggle List", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
    std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED));
    
}