#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <string>
#include <cstdint>

enum {
    W_SDL_INIT_VIDEO,
    W_IMG_INIT_PNG,
};

class WRGB {
public:
    WRGB() = delete;
    WRGB(uint8_t r, uint8_t g, uint8_t b): r(r), g(g), b(b) {}
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

class SDL_Initializer {
public:
    SDL_Initializer() = delete;
    SDL_Initializer(const SDL_Initializer&) = delete;
    SDL_Initializer& operator=(const SDL_Initializer&) = delete;
    
    SDL_Initializer(int flags);
    ~SDL_Initializer();
private:
    int flags;
};

class WWindow {
public:
    WWindow() = delete;
    WWindow(const WWindow&) = delete;
    WWindow& operator=(const WWindow&) = delete;

    WWindow(std::string title, int SCREEN_WIDTH, int SCREEN_HEIGHT, uint32_t flags);
    WWindow(std::string title, int x, int y,int SCREEN_WIDTH, int SCREEN_HEIGHT, uint32_t flags);

    SDL_Window* get() { return window; }

    ~WWindow();
private:
    SDL_Window *window;
};

class WSurface {
public:
    WSurface() = delete;
    WSurface& operator=(const WSurface&) = delete;
    explicit WSurface(std::string path);
    ~WSurface();

    SDL_Surface* get() { return surface; };
    void free();
private:
    SDL_Surface* surface;
};

class WTexture {
public:
    WTexture() = delete;
    WTexture(const WTexture&) = delete;
    WTexture& operator=(const WTexture&) = delete;

    WTexture(std::string path, SDL_Renderer *render);
    WTexture(std::string path, SDL_Renderer *render, WRGB rgb);
    ~WTexture();
    void free();
    SDL_Texture* get() {return texture;}; 

    int width;
    int height;
private:
    SDL_Texture* texture;
};

class WRenderer{
public:
    WRenderer() = delete;
    WRenderer(const WRenderer&) = delete;
    WRenderer& operator=(const WRenderer&) = delete;

    WRenderer(SDL_Window * window, int index, uint32_t flags);
    ~WRenderer();

    SDL_Renderer* get() { return renderer; }
private:
    SDL_Renderer *renderer;
};