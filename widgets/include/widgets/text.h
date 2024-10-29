#include <string>
#include <widgets/widget.h>

#ifndef SDL_WRAPPER_H
#define SDL_WRAPPER_H
#include <SDL_wrapper/wrapper.h>
#endif

class BindText: public Widget {
public:    
    BindText(WRenderer *renderer, std::shared_ptr<std::string> text, SDL_Point &&dst): renderer(renderer), text(text),dst(dst), PRO_DIR(MACRO_PROJECT_DIR) {
        font.reset(new WTTFFont(PRO_DIR + "/widgets/resources/fonts/OpenSans-Light.ttf", 32));
        font_color = {0, 0, 0, 255};
    }
    
    void change_font_size(uint size) { 
        font->set_size(size);
    }
    
    virtual void render() {
        if (text->size() > 0) {
            WTTFSurfaceShaded surface(font.get(), *text, font_color);
            std::unique_ptr<WTexture> text_texture(renderer->create_texture(&surface));
            SDL_Rect rect = { dst.x, dst.y , text_texture->width, text_texture->height};
            text_texture->render(nullptr, &rect);
        }
    };
    
    std::shared_ptr<std::string> get_text() {
        return text;
    }
private:
    WRenderer *renderer;
    SDL_Point dst;
    SDL_Color font_color;
    
    std::shared_ptr<std::string> text;
    std::unique_ptr<WTTFFont> font;
    const std::string PRO_DIR;
};

class Text: public Widget {
public:    
    Text(WRenderer *renderer, std::string &text, SDL_Point &&dst):renderer(renderer), text(text), dst(dst), PRO_DIR(MACRO_PROJECT_DIR) {
        font.reset(new WTTFFont(PRO_DIR + "/widgets/resources/fonts/OpenSans-Light.ttf", 32));
        font_color = {0, 0, 0, 255};
    }
    
    virtual void render() {
        if (text.size() > 0) {
            WTTFSurfaceShaded surface(font.get(), text, font_color);
            std::unique_ptr<WTexture> text_texture(renderer->create_texture(&surface));
            SDL_Rect rect = { dst.x, dst.y , text_texture->width, text_texture->height};
            text_texture->render(nullptr, &rect);
        }
    };
    
    void set_text(std::string &text) {
        this->text = text;
    }
    
    std::string& get_text() {
        return text;
    }
private:
    WRenderer *renderer;
    SDL_Point dst;
    SDL_Color font_color;
    
    std::string text;
    std::unique_ptr<WTTFFont> font;
    const std::string PRO_DIR;
};