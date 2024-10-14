#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <cstdint>
#include <stdexcept>

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

class IMG_Initializer {
public:
    IMG_Initializer() = delete;
    IMG_Initializer(const IMG_Initializer&) = delete;
    IMG_Initializer& operator=(const IMG_Initializer&) = delete;
    
    IMG_Initializer(int flags);
    ~IMG_Initializer();
private:
    int flags;
};

class TTF_Initializer {
public:
    TTF_Initializer(const TTF_Initializer&) = delete;
    TTF_Initializer& operator=(const TTF_Initializer&) = delete;
    
    TTF_Initializer();
    ~TTF_Initializer();
private:
    int flags;
};

class Mix_Initializer {
public:
    Mix_Initializer(const Mix_Initializer&) = delete;
    Mix_Initializer& operator=(const Mix_Initializer&) = delete;
    
    Mix_Initializer();
    ~Mix_Initializer();
};

class WRenderer;

class WWindow {
public:
    WWindow() = delete;
    WWindow(const WWindow&) = delete;
    WWindow& operator=(const WWindow&) = delete;

    WWindow(std::string title, int SCREEN_WIDTH, int SCREEN_HEIGHT, uint32_t flags);
    WWindow(std::string title, int x, int y,int SCREEN_WIDTH, int SCREEN_HEIGHT, uint32_t flags);
    ~WWindow();
    
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
private:
    SDL_Window *window;
    std::shared_ptr<WRenderer> renderer; 
};


class WRenderer {
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

class WTexture {
public:
    WTexture() = delete;
    WTexture(const WTexture&) = delete;
    WTexture& operator=(const WTexture&) = delete;

    WTexture(SDL_Renderer *renderer, SDL_Surface *surface);
    ~WTexture();
    void free();
    SDL_Texture* get() {return texture;}; 

    int width;
    int height;
private:
    SDL_Texture* texture;
};

class WBMPSurface{
public:
    WBMPSurface() = delete;
    WBMPSurface(const WBMPSurface &) = delete;
    WBMPSurface& operator=(const WBMPSurface&) = delete;
    
    WBMPSurface(std::string);
    ~WBMPSurface();
   
    SDL_Surface* get() { return surface; } 
private:
    SDL_Surface* surface; 
};

class WPNGSurface {
public:
    WPNGSurface() = delete;
    WPNGSurface& operator=(const WPNGSurface&) = delete;

    explicit WPNGSurface(std::string path);

    ~WPNGSurface();

    SDL_Surface* get() { return surface; };
    void free();
private:
    SDL_Surface* surface;
};

class WTTFFont {
public:
    WTTFFont() = delete;
    WTTFFont(const WTTFFont&) = delete;
    WTTFFont& operator=(const WTTFFont&) = delete;

    WTTFFont(std::string path, int size);
    ~WTTFFont();

    TTF_Font* get() { return font; } 
private:
    TTF_Font *font;
};

class WTTFSurface {
public:
    WTTFSurface() = delete;
    WTTFSurface(const WTTFSurface&) = delete;
    WTTFSurface& operator=(const WTTFSurface&) = delete;

    WTTFSurface(TTF_Font* , std::string , SDL_Color );
    ~WTTFSurface();

    SDL_Surface* get() { return surface; };
    void free();
private:
    SDL_Surface* surface;
};

class WMUS {
public:
    WMUS() = delete;
    WMUS(const WMUS&) = delete;
    WMUS& operator=(const WMUS&) = delete;
    
    WMUS(std::string path);
    ~WMUS();
    
    Mix_Music* get() { return music; }
private:
    Mix_Music* music;
};

class WWAV {
public:
    WWAV() = delete;
    WWAV(const WWAV&) = delete;
    WWAV& operator=(const WWAV&) = delete;
    
    WWAV(std::string path);
    ~WWAV();
    
    Mix_Chunk* get() { return chunk; }
private:
    Mix_Chunk* chunk;
};

