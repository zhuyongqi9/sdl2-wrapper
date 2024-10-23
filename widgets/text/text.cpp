#include <pthread.h>

#include <iostream>
#include <SDL_wrapper/wrapper.h>
#include <string>
#include <widgets/widget.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const std::string PRO_DIR(MACRO_PROJECT_DIR);

class Text: public Widget {
public:    
    Text(WRenderer *renderer, std::string &text, SDL_Point &&dst):renderer(renderer), dst(dst) {
        this->text = std::make_shared<std::string>(text);
        font.reset(new WTTFFont(PRO_DIR + "/widgets/resources/fonts/OpenSans-Light.ttf", 32));
        font_color = {0, 0, 0, 255};
    }
    
    void change_font_size(uint size) { 
        font->set_size(size);
    }
    
    virtual void render() {
        WTTFSurfaceShaded surface(font.get(), *text, font_color);
        std::unique_ptr<WTexture> text_texture(renderer->create_texture(&surface));
        SDL_Rect rect = { dst.x, dst.y , text_texture->width, text_texture->height};
        text_texture->render(nullptr, &rect);
    };
    
    void bind(std::shared_ptr<std::string> text) {
        this->text = text;
    }
    
    void set_text(std::string &text) {
        this->text = std::make_shared<std::string>(text);
    }
    
    std::string& get_text() {
        return *this->text;
    }
private:
    WRenderer *renderer;
    SDL_Point dst;
    SDL_Color font_color;
    
    std::shared_ptr<std::string> text;
    std::unique_ptr<WTTFFont> font;
};

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
    
    Text bind_widget(renderer.get(), text, {20, 60});
    std::shared_ptr<std::string> name = std::make_shared<std::string>("");
    bind_widget.bind(name);
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