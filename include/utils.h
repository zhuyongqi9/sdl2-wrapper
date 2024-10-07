#include <time.h>
#include <iostream>
#include <cstdint>
#include <chrono>

class Timer {
public:
    Timer() {
        paused_ticks = 0;
        start_ticks = 0;
        started = false;
        paused = false;
    }
    
    void start() {
        start_ticks = now();
        started = true;
        paused = false;
        paused_ticks = 0;
    }
    
    void stop() {
        start_ticks = 0;
        paused_ticks = 0;
        started = false;
        paused = false;
    }
    
    void pause() {
        if (!paused && started) {
            paused_ticks = now() - start_ticks;
            paused = true;
            start_ticks = 0;
        }     
    }
    
    void unpause() {
        if (paused && started) {
            start_ticks = now() - paused_ticks;
            paused_ticks = 0; 
            paused = false;
        }
    }
    
    uint64_t get_ticks() {
        if (started) {
            if (paused) {
                return paused_ticks;
            } else {
                return now() - start_ticks;
            }
        } else {
            return 0;
        }
    }
    
    bool isStarted() { return started; }
    bool isPaused() { return paused; };
private:
    uint64_t paused_ticks;
    uint64_t start_ticks;
    bool paused;
    bool started;
    uint64_t now() {
        using namespace std::chrono;
        uint64_t ticks = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        return ticks;
    }
};

