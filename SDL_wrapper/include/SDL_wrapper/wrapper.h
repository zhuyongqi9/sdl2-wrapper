#include <SDL2/SDL_rect.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <algorithm>
#include <memory>
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


class WRenderer;
class WSurface;
class WTexture;

class WWindow {
public:
    WWindow() = delete;
    WWindow(const WWindow&) = delete;
    WWindow& operator=(const WWindow&) = delete;

    WWindow(std::string title, int SCREEN_WIDTH, int SCREEN_HEIGHT, uint32_t window_flags);
    WWindow(std::string title, int x, int y, int SCREEN_WIDTH, int SCREEN_HEIGHT, uint32_t window_flags);
    ~WWindow();
    
    WRenderer* create_renderer(int index, uint32_t flags);
    
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    SDL_Window* get() {return window;}
private:
    SDL_Window *window;
};

class WRenderer {
public:
    WRenderer() = delete;
    WRenderer(const WRenderer&) = delete;
    WRenderer& operator=(const WRenderer&) = delete;

    WRenderer(WWindow* window, int index, uint32_t flags);
    ~WRenderer();
    
    void set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void clear();
    void present();
    WTexture* create_texture(WSurface *surface); 
    WTexture* create_texture(WSurface &&surface); 
    SDL_Renderer* get() { return renderer; }
private:
    SDL_Renderer *renderer;
};

class WTexture {
public:
    int width;
    int height;
    
    WTexture() = delete;
    WTexture(const WTexture&) = delete;
    WTexture& operator=(const WTexture&) = delete;

    WTexture(WRenderer *, WSurface *);
    WTexture(WRenderer *, WSurface &&);
    ~WTexture();

    void render(SDL_Rect *src, SDL_Rect *dst);
private:
    SDL_Texture* texture;
    WRenderer* renderer;
};

class WSurface {
public:
    virtual SDL_Surface* get() { return nullptr; }
};

class WBMPSurface: public WSurface {
public:
    WBMPSurface() = delete;
    WBMPSurface(const WBMPSurface &) = delete;
    WBMPSurface& operator=(const WBMPSurface&) = delete;
    
    WBMPSurface(std::string);
    WBMPSurface(std::string, uint8_t r, uint8_t g, uint8_t b);
    ~WBMPSurface();
   
    virtual SDL_Surface* get() { return surface; } 
private:
    SDL_Surface* surface; 
};

#ifdef SDL_IMG_ENABLED
#include <SDL2/SDL_image.h>
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

class WPNGSurface: public WSurface {
public:
    WPNGSurface() = delete;
    WPNGSurface& operator=(const WPNGSurface&) = delete;

    explicit WPNGSurface(std::string path);

    ~WPNGSurface();

    virtual SDL_Surface* get() { return surface; };
private:
    SDL_Surface* surface;
};
#endif

#ifdef SDL_TTF_ENABLED
#include <SDL2/SDL_ttf.h>
class TTF_Initializer {
public:
    TTF_Initializer(const TTF_Initializer&) = delete;
    TTF_Initializer& operator=(const TTF_Initializer&) = delete;
    
    TTF_Initializer();
    ~TTF_Initializer();
private:
    int flags;
};

class WTTFFont {
public:
    static const int DEFAULT_FONT_SIZE = 16;
    WTTFFont() = delete;
    WTTFFont(const WTTFFont&) = delete;
    WTTFFont& operator=(const WTTFFont&) = delete;

    WTTFFont(std::string path);
    WTTFFont(std::string path, int size);
    ~WTTFFont();

    TTF_Font* get() { return font; } 
private:
    TTF_Font *font;
    int size;
};

class WTTFSurface: public WSurface {
public:
    WTTFSurface() = delete;
    WTTFSurface(const WTTFSurface&) = delete;
    WTTFSurface& operator=(const WTTFSurface&) = delete;

    WTTFSurface(WTTFFont* font, std::string , SDL_Color);
    ~WTTFSurface();

    virtual SDL_Surface* get() { return surface; };
    void free();
private:
    SDL_Surface* surface;
};
#endif 

#ifdef SDL_MIX_ENABLED
#include <SDL2/SDL_mixer.h>
class Mix_Initializer {
public:
    Mix_Initializer(const Mix_Initializer&) = delete;
    Mix_Initializer& operator=(const Mix_Initializer&) = delete;
    
    Mix_Initializer();
    ~Mix_Initializer();
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
#endif

