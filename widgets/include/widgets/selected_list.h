#ifndef WRAPPER_H
#define WRAPPER_H
#include <SDL_wrapper/wrapper.h>
#endif
#include <iostream>
#include <vector>

class SelectedList {
public:
    SelectedList(WRenderer *renderer, std::vector<std::string> items, SDL_Point dst):renderer(renderer), items(items), dst(dst), PRO_DIR(MACRO_PROJECT_DIR) {
        default_item = "None";
        font_default.reset(new WTTFFont(PRO_DIR + "/widgets/selected_list/OpenSans-Light.ttf", 20 * SCALE));
        default_width = 214 * SCALE;
        default_height = 32 * SCALE;
        color_border_item_default = {37,84,149,255};
        
        font_color = {0, 0, 0, 255};
        icon.reset(renderer->create_texture(WPNGSurface(PRO_DIR + "/widgets/selected_list/test.png")));
        
        item_font.reset(new WTTFFont(PRO_DIR + "/widgets/selected_list/OpenSans-Light.ttf", 17 * SCALE));
        item_width = (214 + icon->width) * SCALE;
        item_height = 28 * SCALE;
        color_bg_item_selected = {234,242,250, 255};
        cnt_item_shown = items.size();
        index_item_selected = -1;
        expend = true;
        
        int x = dst.x;
        int y = dst.y + default_height;
        arr_rect_item_shown.resize(cnt_item_shown);
        for(int i = 0; i < cnt_item_shown; i++) {
            arr_rect_item_shown[i] = {x, y, item_width, item_height};
            y += item_height;
        }
        y += item_height;
        widget_width = item_width;
        widget_height = item_height * cnt_item_shown;
    }
    
    
    void handle_event(SDL_Event &e) {
        SDL_Point point;
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            SDL_GetMouseState(&point.x, &point.y);
            if (in_region(point, rect_icon)) {
                std::cout << "clicked: " << expend << std::endl;
                expend = !expend;
            } else {
                if (expend) {
                    SDL_GetMouseState(&point.x, &point.y);
                    for (int i = 0; i < cnt_item_shown; i++) {
                        if (in_region(point, arr_rect_item_shown[i])) {
                            std::cout << "selected: " << i << std::endl;
                            index_item_selected = i;
                            expend = false;
                            default_item = items[i];
                            break;
                        }
                    }
                }
            }
        } else if (e.type == SDL_MOUSEMOTION) {
            if (expend) {
                SDL_GetMouseState(&point.x, &point.y);
                for (int i = 0; i < cnt_item_shown; i++) {
                    if (in_region(point, arr_rect_item_shown[i])) {
                        index_item_selected = i;
                        std::cout << "selected: " << i << std::endl;
                        break;
                    }
                }
            }
        }
    }
    
    void render() {
        // draw default item
        int level_width = 5 * SCALE;
        std::unique_ptr<WTexture> texture_default_item(renderer->create_texture(WTTFSurfaceBlended(font_default.get(), default_item, font_color))); 
        SDL_Rect rect_default_item_text = {dst.x + level_width, dst.y, texture_default_item->width, texture_default_item->height};
        texture_default_item->render(nullptr, &rect_default_item_text);
            
        SDL_Rect default_item_border = {dst.x, dst.y, default_width, default_height};
        renderer->draw_rect(&default_item_border, color_border_item_default);
        
        //draw arrow icon
        this->rect_icon = {dst.x + default_width, dst.y, icon->width * SCALE, icon->height * SCALE};
        icon->render(nullptr, &rect_icon);
        
        // render items
        if (expend) {
            // render item boarder
            SDL_Rect widget_border = {dst.x, dst.y + default_height, widget_width, widget_height};
            renderer->draw_rect(&widget_border, color_border_item_default);
            
            for (int i = 0; i < cnt_item_shown; i++ ) {
                if (i == index_item_selected) {
                    SDL_Rect selected_item_bg_rect = { arr_rect_item_shown[i].x + 1, arr_rect_item_shown[i].y, arr_rect_item_shown[i].w - 2, arr_rect_item_shown[i].h-1};
                    renderer->fill_rect(&selected_item_bg_rect, color_bg_item_selected);
                }
                std::unique_ptr<WTexture> item(renderer->create_texture(WTTFSurfaceBlended(item_font.get(), items[i], font_color))); 
                int x = arr_rect_item_shown[i].x, y = arr_rect_item_shown[i].y;
                SDL_Rect rect_item ={x + level_width + level_width, y, item->width, item->height}; 
                item->render(nullptr, &rect_item);
                //renderer->draw_rect(&arr_rect_item_shown[i], {0, 0, 0, 255});
            }
        }
    }
    
private:
    const std::string PRO_DIR;
    
    WRenderer *renderer;
    SDL_Point dst;
    int widget_width;
    int widget_height;
    
    std::vector<std::string> items; 
    std::unique_ptr<WTTFFont> item_font;
    int item_width;
    int item_height;
    int cnt_item_shown;
    int index_item_selected;
    std::vector<SDL_Rect> arr_rect_item_shown;
    SDL_Color color_bg_item_selected;
    
    SDL_Color color_border_item_default;
    
    std::string default_item;
    std::unique_ptr<WTTFFont> font_default;
    int default_width;
    int default_height;
    
    SDL_Color font_color;
    bool expend;
    
    std::unique_ptr<WTexture> icon;
    SDL_Rect rect_icon;
    bool in_region(SDL_Point &point, SDL_Rect &region) {
        return (point.x >= region.x && point.x <= region.x + region.w)
        && (point.y >= region.y && point.y <= region.y + region.h);
    }
};
