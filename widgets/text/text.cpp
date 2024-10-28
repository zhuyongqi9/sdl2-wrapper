#include <pthread.h>
#include <widgets/text.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const std::string PRO_DIR(MACRO_PROJECT_DIR);

int main(int argc, char **argv) {
    SDL_Initializer sdl_initializer(SDL_INIT_AUDIO);
    TTF_Initializer ttf_initializer;
    
    std::unique_ptr<WWindow> window(new WWindow("Text", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
    std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED));
    
    bool quit = false;
    SDL_Event e;
    
    std::vector<Widget *> widgets(0);
    
    std::string text("This is test text");
    Text text_widget(renderer.get(), text, {20, 20});
    
    std::shared_ptr<std::string> name = std::make_shared<std::string>("");
    BindText bind_widget(renderer.get(), name, {20, 60});
    *name = "zhuyognqi";
    
    widgets.push_back(&text_widget);
    widgets.push_back(&bind_widget);
    
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }         
        }

        renderer->set_color(255, 255, 255, 255);
        renderer->clear();
        for (auto &widget : widgets) {
            widget->render();
        }
        renderer->present();
    }
    return 0;
}