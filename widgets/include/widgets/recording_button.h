#ifndef SDL_WRAPPER_H
#define SDL_WRAPPER_H
#include <SDL_wrapper/wrapper.h>
#endif
#include "SDL2/SDL_events.h"


class RecordingButton {
public: 
    RecordingButton(WRenderer *renderer, SDL_Point dst): renderer(renderer), dst(dst), PRO_DIR(MACRO_PROJECT_DIR) {
        status = stopped;
        
        WPNGSurface stopped_icon_surface(WPNGSurface(PRO_DIR + "/widgets/recording_button/recording_button.png"));
        stopped_icon_surface.set_color_key(0, 0, 0);
        stopped_icon.reset(renderer->create_texture(&stopped_icon_surface));
        
        WPNGSurface recording_icon_surface(WPNGSurface(PRO_DIR + "/widgets/recording_button/recording_button_recording.png"));
        recording_icon_surface.set_color_key(0, 0, 0);
        recording_icon.reset(renderer->create_texture(&recording_icon_surface));
        
        region = { dst.x, dst.y, width, height};
    }
    
    typedef void (*start_recording_callback)();
    void set_start_recording_callback(start_recording_callback start) {
        this->start_recording = start;
    }
    
    typedef void (*stop_recording_callback)();
    void set_stop_recording_callback(stop_recording_callback stop) {
        this->stop_recording = stop;
    }
    
    void handle_event(SDL_Event &e) {
        SDL_Point point;
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            SDL_GetMouseState(&point.x, &point.y);
            if (in_region(point, region))  {
                if (status == stopped) {
                    status = recording;
                    if (start_recording != nullptr)
                        start_recording();
                } else if (status == recording) {
                    status = stopped;
                    if (stop_recording != nullptr)
                        stop_recording();
                }
            }
        } 
    }
    
    void render() {
        if (status == stopped) {
            stopped_icon->render(nullptr, &region);
        } else if (status == recording) {
            recording_icon->render(nullptr, &region);
        }
    }
    
    enum state {
        recording,
        paused,
        stopped
    };
    
    void set_state(state status) { this->status = stopped; }
private:
    
    const std::string PRO_DIR;
    
    WRenderer *renderer; 
    std::unique_ptr<WTexture> stopped_icon;
    std::unique_ptr<WTexture> recording_icon;
    SDL_Point dst;
    SDL_Rect region;
    
    state status;
    
    //1080 * 658
    int width = 108;
    int height = 108;
    
    start_recording_callback start_recording;
    stop_recording_callback stop_recording;
    
    bool in_region(SDL_Point &point, SDL_Rect &region) {
        return (point.x >= region.x && point.x <= region.x + region.w)
        && (point.y >= region.y && point.y <= region.y + region.h);
    }
};


