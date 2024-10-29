#ifndef SDL_WRAPPER_H
#define SDL_WRAPPER_H
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_stdinc.h"
#include "SDL2/SDL_surface.h"
#include <SDL_wrapper/wrapper.h>
#endif
#include <exception>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <vector>
#include <widgets/selected_list.h>
#include <widgets/recording_button.h>
#include <algorithm>

const std::string PRO_DIR(MACRO_PROJECT_DIR);

const int SCREEN_WIDTH = 650 * SCALE;
const int SCREEN_HEIGHT = 300 * SCALE;

int available_recording_deivce_cnt = 0;

const int MAX_RECORDING_DEVICES = 10;

void call_audio_recording_callback (void *userdata, uint8_t *stream, int len);

class Recorder {
public: 
    enum state {
    selected,
    recording,
    stopped,
    };
    Recorder() {
        int cnt = SDL_GetNumAudioDevices(SDL_TRUE);
        if (cnt > 0) {
            recording_devices.resize(cnt);
            for (int i = 0; i < cnt; i++) {
                recording_devices[i] = SDL_GetAudioDeviceName(i, SDL_TRUE);
            }
        } else {
            recording_devices.resize(0);
        }
        selected_device = -1;
        status = stopped;
    }
    
    void select_device(uint32_t device) { 
        if (status == stopped) {
            SDL_AudioSpec desired_recording_spec;
            SDL_zero(desired_recording_spec);
            desired_recording_spec.freq = 44100;
            desired_recording_spec.format = AUDIO_F32;
            desired_recording_spec.channels = 2;
            desired_recording_spec.samples = 4096;
            desired_recording_spec.userdata = this;
            desired_recording_spec.callback = call_audio_recording_callback;
            
            selected_device = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(device, SDL_TRUE), SDL_TRUE, &desired_recording_spec, &received_recording_spec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
            if (selected_device == 0) {
                throw std::runtime_error("failed to open recording audio");
            }
            
            int bytesPerSample = received_recording_spec.channels * (SDL_AUDIO_BITSIZE(received_recording_spec.format) / 8);
            int bytesPerSecond = received_recording_spec.freq * bytesPerSample;
            
            buffer_byte_size = RECORDING_BUFFER_SECONDS * bytesPerSecond;
            buffer_max_pos = MAX_RECORDING_SECONDS * bytesPerSecond;
            recording_buffer_len = buffer_byte_size;
            recording_buffer.reset(new uint8_t[buffer_byte_size]);
            
            status = selected;
        }
    }
    
    void start_recording() {
        if (status == selected) {
            std::memset(recording_buffer.get(), 0, buffer_byte_size);
            
            buffer_byte_pos = 0;
            SDL_PauseAudioDevice(selected_device, SDL_FALSE);
            status = recording;
        } else {
            throw std::runtime_error("must selected reocording device before starting recording");
        }
    }
    
    bool finished() {
        return (buffer_byte_pos > buffer_max_pos);
    }
    
    void stop_recording() {
        if (status == recording) {
            SDL_PauseAudioDevice(selected_device, SDL_TRUE);
        } else {
            throw std::runtime_error("must selected reocording device before stopping reocrding");
        }
        status = stopped;
    }
    
    void audio_recording_callback(void *userdata, uint8_t *stream, int len) {
        std::memcpy(recording_buffer.get() + buffer_byte_pos, stream, len);
        buffer_byte_pos += len;
    }
    
    void handle_event(SDL_Event &e) {
    }
    
    std::vector<std::string> recording_devices;
    std::unique_ptr<uint8_t> recording_buffer;
    uint32_t recording_buffer_len;
    uint32_t buffer_byte_pos;
    
    state get_state() {return status;}
private:
    static const int MAX_RECORDING_SECONDS = 5;
    static const int RECORDING_BUFFER_SECONDS = MAX_RECORDING_SECONDS + 1;
    SDL_AudioSpec received_recording_spec;
    uint32_t buffer_byte_size;
    uint32_t buffer_max_pos;
    
    uint32_t selected_device;
    
    uint32_t event_base;
    
    state status;
};

void call_audio_recording_callback (void *userdata, uint8_t *stream, int len) {
    Recorder *recorder = static_cast<Recorder *>(userdata);
    recorder->audio_recording_callback(userdata, stream, len);
}

/** 
class Player {
public: 
    Player() {
        int cnt = SDL_GetNumAudioDevices(SDL_FALSE);
        playing_devices.resize(cnt);
        for (int i = 0; i < cnt; i++) {
            playing_devices[i] = SDL_GetAudioDeviceName(i, SDL_FALSE);
        }
        
    }
    
    void select_device(int device) {
        SDL_AudioSpec desired_playback_spec;
        desired_playback_spec.freq = 44100;
        desired_playback_spec.format = AUDIO_F32; 
        desired_playback_spec.channels = 2;
        desired_playback_spec.samples = 4096;
        desired_playback_spec.callback = audio_playback_callback;
        playback_device_id = SDL_OpenAudioDevice(NULL, 0, &desired_playback_spec, &received_playback_spec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
        if (playback_device_id == 0) {
            throw std::runtime_error("failed to open playback audio");
        }
        
    }
    std::vector<std::string> playing_devices;
private:
};
*/

//void audio_playback_callback(void *userdata, uint8_t *stream, int len) {
//    std::memcpy(stream, &recording_buffer[buffer_byte_pos], len);
//    buffer_byte_pos += len;
//}

Recorder *recorder;
void selected_callback(int index) {
    std::cout << "callback selected list" << std::endl;
    recorder->select_device(index);
}

void start_recording_callback() {
    std::cout << "callback start recording" << std::endl;
    recorder->start_recording();
}

void stop_recording_callback() {
    std::cout << "callback stop recording" << std::endl;
    recorder->stop_recording();
}

class Grid {
public: 
    Grid(WRenderer *renderer, int width, int height):renderer(renderer), width(width), height(height) {
    } 
    
    void render() {
        // draw grid
        SDL_Rect dst = {0, 83, width, height};
        //renderer->fill_rect(&dst, {0,0,0,255});
        
        // drawline
        int len = recorder->recording_buffer_len;
        int step = len / width;
        renderer->set_color(255, 255, 0, 255);
        renderer->set_color(135	,78	,224,255	);
        
        for (int i = 0, j=0; i < recorder->buffer_byte_pos; i+=step,j++) {
            renderer->draw_line({j, SCREEN_HEIGHT - (recorder->recording_buffer.get()[i] * height / 255)}, {j, SCREEN_HEIGHT});
        }
    }
    
    bool flag = false;
private:
    WRenderer *renderer;
    int GRID_SIZE = 10;
    
    int width;
    int height;
    
    int x_cnt = width / GRID_SIZE;
    int y_cnt = height/ GRID_SIZE;
};


int main(int argc, char **argv) {
    try {
        SDL_Initializer sdl_initializer(SDL_INIT_AUDIO);
        TTF_Initializer ttf_inirializer;
        Mix_Initializer mix_initializer;
        
        std::unique_ptr<WWindow> window(new WWindow("Audio Recorder & Player", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN|SDL_WINDOW_ALLOW_HIGHDPI));
        std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED));
        
        recorder = new Recorder;
        
        SelectedList recording_devices_list(renderer.get(), recorder->recording_devices, {7, 24});
        recording_devices_list.set_selected_callback(selected_callback);
        
        RecordingButton recording_button(renderer.get(), {SCREEN_WIDTH/2, 20});
        recording_button.set_start_recording_callback(start_recording_callback);
        recording_button.set_stop_recording_callback(stop_recording_callback);
        
        Grid grid(renderer.get(), SCREEN_WIDTH, 217);
//        Player player;
//        SelectedList playing_devices_list(renderer.get(), player.playing_devices, {SCREEN_WIDTH/ 2, 20});
        
        bool quit = false;
        SDL_Event e;
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
                recording_devices_list.handle_event(e);
                recording_button.handle_event(e);
//                playing_devices_list.handle_event(e);
            }
            
            if (recorder->get_state() == Recorder::recording) {
                if (recorder->finished()) {
                    std::cout << "finished" << std::endl;
                    recorder->stop_recording();
                    for (int i = 0; i < 100; i++) {
                        std::cout << " " << int(recorder->recording_buffer.get()[i]);
                    }
                    std::cout << std::endl;
                    std::cout << (int)*(std::max_element(recorder->recording_buffer.get(), recorder->recording_buffer.get() + recorder->recording_buffer_len)) << std::endl;;
                    std::cout << (int)*(std::min_element(recorder->recording_buffer.get(), recorder->recording_buffer.get() + recorder->recording_buffer_len)) << std::endl;;
                    
                    recording_button.set_state(RecordingButton::stopped);
                }
            }
            
            renderer->set_color(255, 255, 255, 255);
//            renderer->set_color(0, 0, 0, 255);
            renderer->clear();
            recording_devices_list.render();
            recording_button.render();
            grid.render();
//            playing_devices_list.render();
            renderer->present();
        }
        
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

/** int main(int argc, char **argv) {
    try {
        SDL_Initializer sdl_initializer(SDL_INIT_AUDIO);
        Mix_Initializer mix_initializer;
        
        std::unique_ptr<WWindow> window(new WWindow("Audio Recorder & Player", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
        std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED));
        
        WTTFFont font(PRO_DIR + "/audio_recorder/OpenSans-Regular.ttf", 48);
        
        // no zero get recording deivce
        available_recording_deivce_cnt = SDL_GetNumAudioDevices(1);
        
        RecordingState current_state = SELECTING_DEVICE;
        SDL_AudioDeviceID recording_device_id = 0;
        SDL_AudioDeviceID playback_device_id = 0;
        
        std::string state_text("Selecting device");
        SDL_Event e;
        bool quit = false;
        while (!quit) {
            int x,y;
            x = y = 0;
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
                
                switch (current_state) {
                    case SELECTING_DEVICE:
                        if (e.type == SDL_KEYDOWN) {
                            SDL_Keycode code = e.key.keysym.sym;
                            if (code >= SDLK_0 && code <= SDLK_9) {
                                int index = code - SDLK_0;
                                state_text = "Selected Device: " + std::to_string(index); 
                                SDL_AudioSpec desired_recording_spec;
                                SDL_zero(desired_recording_spec);
                                desired_recording_spec.freq = 44100;
                                desired_recording_spec.format = AUDIO_F32;
                                desired_recording_spec.channels = 2;
                                desired_recording_spec.samples = 4096;
                                desired_recording_spec.callback = audio_recording_callback;
                                
                                recording_device_id = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(index, 1), 1, &desired_recording_spec, &received_recording_spec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
                                if (recording_device_id == 0) {
                                    throw std::runtime_error("failed to open recording audio");
                                }
                                
                                SDL_AudioSpec desired_playback_spec;
                                desired_playback_spec.freq = 44100;
                                desired_playback_spec.format = AUDIO_F32; 
                                desired_playback_spec.channels = 2;
                                desired_playback_spec.samples = 4096;
                                desired_playback_spec.callback = audio_playback_callback;
                                playback_device_id = SDL_OpenAudioDevice(NULL, 0, &desired_playback_spec, &received_playback_spec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
                                if (playback_device_id == 0) {
                                    throw std::runtime_error("failed to open playback audio");
                                }
                                
                                int bytesPerSample = received_recording_spec.channels * (SDL_AUDIO_BITSIZE(received_recording_spec.format) / 8);
                                int bytesPerSecond = received_recording_spec.freq * bytesPerSample;
                                
                                buffer_byte_size = RECORDING_BUFFER_SECONDS * bytesPerSecond;
                                buffer_max_pos = MAX_RECORDING_SECONDS * bytesPerSecond;
                                recording_buffer = new uint8_t[buffer_byte_size];
                                std::memset(recording_buffer, 0, buffer_byte_size);
                                //state_text = "Press 1 to record for 5 seconds";
                                current_state = STOPPED;
                            }
                        }
                        break;
                    case STOPPED:
                        if (e.type == SDL_KEYDOWN) {
                            if (e.key.keysym.sym == SDLK_1) {
                                buffer_byte_pos = 0;
                                SDL_PauseAudioDevice(recording_device_id, SDL_FALSE);
                                state_text = "Recording...";
                                current_state = RECORDING;
                            }
                        }
                        break;
                    case RECORDED:
                        if (e.type == SDL_KEYDOWN) {
                            if (e.key.keysym.sym == SDLK_1) {
                                buffer_byte_pos = 0;
                                SDL_PauseAudioDevice(playback_device_id, SDL_FALSE);
                                state_text = "Playing";
                                current_state = PLAYBACK;
                            } else if (e.key.keysym.sym == SDLK_2) {
                                buffer_byte_pos = 0;
                                SDL_PauseAudioDevice(recording_device_id, SDL_FALSE);
                                state_text = "Recording...";
                                current_state = RECORDING;
                            }
                        }
                        break;
                }
            }
            
            if (current_state == RECORDING) {
                    SDL_LockAudioDevice(recording_device_id);
                    if (buffer_byte_pos > buffer_max_pos) {
                        SDL_PauseAudioDevice(recording_device_id, SDL_TRUE);
                        state_text = "Press 1 to playback, 2 to record again";
                        current_state = RECORDED;
                    }
                    SDL_UnlockAudioDevice(recording_device_id);
            } else if (current_state == PLAYBACK) {
                    SDL_LockAudioDevice(playback_device_id);
                    if (buffer_byte_pos > buffer_max_pos) {
                        SDL_PauseAudioDevice(playback_device_id, SDL_TRUE);
                        state_text = "Press 1 to playback, 2 to record again";
                        current_state = RECORDED;
                    }
                    SDL_UnlockAudioDevice(playback_device_id);
            }
            
            SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 255);
            SDL_RenderClear(renderer.get());
            WTTFSurface surface_promot(font.get(), "Available Recording Devices: " + std::to_string(available_recording_deivce_cnt), {0,0,0,255});
            WTexture promot(renderer.get(), surface_promot.get());
            SDL_Rect dst = {x, y, promot.width, promot.height};
            SDL_RenderCopy(renderer.get(), promot.get(), NULL, &dst);
            y += promot.height;
            
            for (int i = 0; i < available_recording_deivce_cnt; i++) {
                WTTFSurface surface_device_name(font.get(), "index: " + std::to_string(i) + SDL_GetAudioDeviceName(i, SDL_TRUE), {0,0,0,255});
                WTexture device_name(renderer.get(), surface_device_name.get());
                dst = {x, y, device_name.width, device_name.height};
                SDL_RenderCopy(renderer.get(), device_name.get(), NULL, &dst);
                y += device_name.height;
            }
            
            WTTFSurface surface_state_text(font.get(), state_text, {0,0,0,255});
            WTexture state_text(renderer.get(), surface_state_text.get());
            dst = {x, y, state_text.width, state_text.height};
            SDL_RenderCopy(renderer.get(), state_text.get(), NULL, &dst);
            
            SDL_RenderPresent(renderer.get());
        }
        
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}
*/