#include <SDL2/SDL_clipboard.h>
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

    WWindow(std::string title, int WIDNOW_WITH, int WINDOW_HEIGHT, uint32_t window_flags);
    WWindow(std::string title, int x, int y, int WIDNOW_WITH, int WINDOW_HEIGHT, uint32_t window_flags);
    ~WWindow();
    
    WRenderer* create_renderer(int index, uint32_t flags);
    
    int window_width;
    int window_height;
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
    void set_target(WTexture *);
    void clear();
    void present();
    void draw_rect(SDL_Rect *rect);
    void draw_rect(SDL_Rect *rect, SDL_Color color);
    void fill_rect(SDL_Rect *rect);
    void fill_rect(SDL_Rect *rect, SDL_Color color);
    void draw_line(SDL_Point *start, SDL_Point *end);
    void draw_line(SDL_Point &&start, SDL_Point &&end);
    void draw_point(SDL_Point *point);
    void draw_point(SDL_Point point);
    void set_viewport(SDL_Rect *rect);
    void set_viewport(SDL_Rect &&rect);
    
    std::unique_ptr<WTexture> create_texture(WSurface *surface); 
    std::unique_ptr<WTexture> create_texture(WSurface &&surface); 
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

    explicit WTexture(WRenderer *renderer, SDL_Texture *texture): renderer(renderer), texture(texture){}
    explicit WTexture(WRenderer *, WSurface *);
    explicit WTexture(WRenderer *, WSurface &&);
    ~WTexture();

    void render(const SDL_Rect *src, const SDL_Rect *dst);
    void renderEx(const SDL_Rect *src, const SDL_Rect *dst, double angle, SDL_Point *point, SDL_RendererFlip flip);
    void set_color_mod(int8_t r, int8_t g, int8_t b);
    void set_alpha_mod(uint8_t alpha);
    
    SDL_Texture* get() { return texture; }
private:
    SDL_Texture* texture;
    WRenderer* renderer;
};

class WSurface {
public:
    virtual ~WSurface(){
        if (surface != nullptr) {
            SDL_FreeSurface(surface);
        }
    } 
    
    explicit WSurface():surface(nullptr) {}
    explicit WSurface(SDL_Surface *surface): surface(surface){}
    
    virtual SDL_Surface* get() { return surface; }
private:
    SDL_Surface *surface;
};

class WBMPSurface: public WSurface {
public:
    WBMPSurface() = delete;
    WBMPSurface(const WBMPSurface &) = delete;
    WBMPSurface& operator=(const WBMPSurface&) = delete;
    
    explicit WBMPSurface(std::string);
    ~WBMPSurface();
    
    void set_color_key(uint8_t r, uint8_t g, uint8_t b);
   
    virtual SDL_Surface* get() { return surface; } 
private:
    SDL_Surface* surface; 
};

class WClipboardText {
public:
    WClipboardText(const WClipboardText&) = delete; 
    WClipboardText& operator=(const WClipboardText &) = delete;
    
    WClipboardText() {
        this->str = SDL_GetClipboardText(); 
    }
    
    char* get() {return str;}
    
    ~WClipboardText() {
        if (str != nullptr) {
            SDL_free(str);
        }
    }
private:
    char *str;
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
    
    void set_color_key(uint8_t r, uint8_t g, uint8_t b);

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

    WTTFFont(std::string path, uint size);
    ~WTTFFont();
    
    void set_size(uint size);

    TTF_Font* get() { return font; } 
private:
    TTF_Font *font;
    uint size;
};

class WTTFSurfaceSolid: public WSurface {
public:
    WTTFSurfaceSolid() = delete;
    WTTFSurfaceSolid(const WTTFSurfaceSolid&) = delete;
    WTTFSurfaceSolid& operator=(const WTTFSurfaceSolid&) = delete;

    WTTFSurfaceSolid(WTTFFont* font, std::string &&, SDL_Color);
    WTTFSurfaceSolid(WTTFFont* font, std::string &, SDL_Color);
    WTTFSurfaceSolid(WTTFFont* font, std::string &, SDL_Color, int wrap_length);
    ~WTTFSurfaceSolid();

    virtual SDL_Surface* get() { return surface; };
    void free();
private:
    SDL_Surface* surface;
};

class WTTFSurfaceShaded: public WSurface {
public:
    WTTFSurfaceShaded() = delete;
    WTTFSurfaceShaded(const WTTFSurfaceShaded&) = delete;
    WTTFSurfaceShaded& operator=(const WTTFSurfaceShaded&) = delete;

    WTTFSurfaceShaded(WTTFFont* font, std::string &&, SDL_Color);
    WTTFSurfaceShaded(WTTFFont* font, std::string &, SDL_Color);
    WTTFSurfaceShaded(WTTFFont* font, std::string &, SDL_Color, int wrap_length);
    ~WTTFSurfaceShaded();

    virtual SDL_Surface* get() { return surface; };
    void free();
private:
    SDL_Surface* surface;
};

class WTTFSurfaceBlended: public WSurface {
public:
    WTTFSurfaceBlended() = delete;
    WTTFSurfaceBlended(const WTTFSurfaceBlended&) = delete;
    WTTFSurfaceBlended& operator=(const WTTFSurfaceBlended&) = delete;

    WTTFSurfaceBlended(WTTFFont* font, std::string &&, SDL_Color);
    WTTFSurfaceBlended(WTTFFont* font, std::string &, SDL_Color);
    WTTFSurfaceBlended(WTTFFont* font, std::string &, SDL_Color, int wrap_length);
    ~WTTFSurfaceBlended();

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
    void play(int loops);
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
    
    void play_channel(int channel, int loops);
    Mix_Chunk* get() { return chunk; }
private:
    Mix_Chunk* chunk;
};
#endif

