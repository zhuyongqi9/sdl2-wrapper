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
    Text(WRenderer *renderer, std::string &text, SDL_Point &&dst):renderer(renderer), text(text), dst(dst) {
        font.reset(new WTTFFont(PRO_DIR + "/widgets/resources/fonts/OpenSans-Light.ttf", 32));
        font_color = {0, 0, 0, 255};
    }
    
    void change_font_size(uint size) { 
        font->set_size(size);
    }
    
    virtual void render() {
        WTTFSurfaceShaded surface(font.get(), text, font_color);
        std::unique_ptr<WTexture> text_texture(renderer->create_texture(&surface));
        SDL_Rect rect = { dst.x, dst.y , text_texture->width, text_texture->height};
        text_texture->render(nullptr, &rect);
    };
private:
    WRenderer *renderer;
    SDL_Point dst;
    SDL_Color font_color;
    
    std::string &text;
    std::unique_ptr<WTTFFont> font;
};

int main(int argc, char **argv) {
    SDL_Initializer sdl_initializer(SDL_INIT_AUDIO);
    TTF_Initializer ttf_initializer;
    
    std::unique_ptr<WWindow> window(new WWindow("Text", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
    std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED));
    
    bool quit = false;
    SDL_Event e;
    
    std::string text("This is test text");
    Text text_widget(renderer.get(), text, {20, 20});
    
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }         
        }

        renderer->set_color(255, 255, 255, 255);
        renderer->clear();
        text_widget.render();
        renderer->present();
    }
    return 0;
}