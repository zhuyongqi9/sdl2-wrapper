#include <SDL_wrapper/wrapper.h>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <cstring>

const std::string PRO_DIR(MACRO_PROJECT_DIR);

const int SCREEN_WIDTH = 800 * SCALE;
const int SCREEN_HEIGHT = 600 * SCALE;

int available_recording_deivce_cnt = 0;
SDL_AudioSpec received_recording_spec;
SDL_AudioSpec received_playback_spec;

uint8_t *recording_buffer = nullptr;
uint32_t buffer_byte_size = 0;
uint32_t buffer_byte_pos = 0;
uint32_t buffer_max_pos = 0;

class Recorder {
public: 
    Recorder() {
        int cnt = SDL_GetNumAudioDevices(SDL_TRUE);
        recording_devices.resize(cnt);
        for (int i = 0; i < cnt; i++) {
            recording_devices[i] = SDL_GetAudioDeviceName(i, SDL_TRUE);
        }
    }
    
    std::vector<std::string> recording_devices;
private:
};

class Player {
    public: 
        Player() {
            
        }
    private:
        std::vector<std::string> playing_devices;
};

void audio_recording_callback(void *userdata, uint8_t *stream, int len) {
    std::memcpy(&recording_buffer[buffer_byte_pos], stream, len);
    buffer_byte_pos += len;
}

void audio_playback_callback(void *userdata, uint8_t *stream, int len) {
    std::memcpy(stream, &recording_buffer[buffer_byte_pos], len);
    buffer_byte_pos += len;
}

int main(int argc, char **argv) {
    try {
        SDL_Initializer sdl_initializer(SDL_INIT_AUDIO);
        Mix_Initializer mix_initializer;
        
        std::unique_ptr<WWindow> window(new WWindow("Audio Recorder & Player", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
        std::unique_ptr<WRenderer> renderer(window->create_renderer(-1, SDL_RENDERER_ACCELERATED));
        
        WTTFFont font(PRO_DIR + "/audio_recorder/OpenSans-Regular.ttf", 48);
        
        
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