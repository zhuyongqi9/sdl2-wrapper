#include "SDL_wrapper/wrapper.h"
#include "SDL2/SDL_blendmode.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_pixels.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_ttf.h"

SDL_Initializer::SDL_Initializer(int flags) {
    if (SDL_Init(flags) < 0) {
        throw std::runtime_error("Failed to initialize SDL, error: " + std::string(SDL_GetError()));
    }
} 

SDL_Initializer::~SDL_Initializer() {
    SDL_Quit();
}

WWindow::WWindow(std::string title, int SCREEN_WIDTH, int SCREEN_HEIGHT, uint32_t flags) {
    SDL_Window* window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, flags);
    if (window == nullptr) {
        throw std::runtime_error("failed to create window error:" + std::string(SDL_GetError()));
    }
    this->window = window;
}

WWindow::WWindow(std::string title, int x, int y,int SCREEN_WIDTH, int SCREEN_HEIGHT, uint32_t flags) {
    SDL_Window *window = SDL_CreateWindow(title.c_str(), x, y, SCREEN_WIDTH, SCREEN_HEIGHT, flags);
    if (window == nullptr) {
        throw std::runtime_error("failed to create window error:" + std::string(SDL_GetError()));
    }
    this->window = window;
}

WWindow::~WWindow() {
    if (window != nullptr) {
        SDL_DestroyWindow(window);
    }
}

WRenderer* WWindow::create_renderer(int index, uint32_t flags) {
    return new WRenderer(this, index, flags);
}

WRenderer::WRenderer(WWindow * window, int index, uint32_t flags) {
    SDL_Renderer *renderer = SDL_CreateRenderer(window->get(), index, flags);
    if (renderer == nullptr) {
        throw std::runtime_error("Failed to create renderer, error: " + std::string(SDL_GetError()));
    }
    this->renderer = renderer;
}

WRenderer::~WRenderer() {
    if (renderer != nullptr) {
        SDL_DestroyRenderer(renderer);
    }
}

WTexture* WRenderer::create_texture(WSurface *surface) {
    return new WTexture(this, surface);
}

WTexture* WRenderer::create_texture(WSurface &&surface) {
    return new WTexture(this, &surface);
}

void WRenderer::set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void WRenderer::clear() {
    SDL_RenderClear(renderer);
}

void WRenderer::present() {
    SDL_RenderPresent(renderer);
}

void WRenderer::fill_rect(SDL_Rect *rect) {
    SDL_RenderFillRect(renderer, rect);
}

void WRenderer::fill_rect(SDL_Rect *rect, SDL_Color color) {
    uint8_t ori_r, ori_g, ori_b, ori_a;
    SDL_GetRenderDrawColor(renderer, &ori_r, &ori_g, &ori_b, &ori_a);
    this->set_color(color.r, color.g, color.b, color.a);
    fill_rect(rect);
    this->set_color(ori_r, ori_g, ori_b, ori_a);
}

void WRenderer::draw_rect(SDL_Rect *rect) {
    SDL_RenderDrawRect(renderer, rect);
}

void WRenderer::draw_rect(SDL_Rect *rect, SDL_Color color) {
    uint8_t ori_r, ori_g, ori_b, ori_a;
    SDL_GetRenderDrawColor(renderer, &ori_r, &ori_g, &ori_b, &ori_a);
    this->set_color(color.r, color.g, color.b, color.a);
    draw_rect(rect);
    this->set_color(ori_r, ori_g, ori_b, ori_a);
}

void WRenderer::draw_line(SDL_Point *start, SDL_Point *end) {
    SDL_RenderDrawLine(renderer, start->x, start->y, end->x, end->y);
}

void WRenderer::draw_line(SDL_Point &&start, SDL_Point &&end) {
    SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
}

void WRenderer::draw_point(SDL_Point *point) {
    SDL_RenderDrawPoint(renderer, point->x, point->y);
}

void WRenderer::draw_point(SDL_Point point) {
    SDL_RenderDrawPoint(renderer, point.x, point.y);
}

void WRenderer::set_viewport(SDL_Rect *rect) {
    SDL_RenderSetViewport(renderer, rect);
}

void WRenderer::set_viewport(SDL_Rect &&rect) {
    SDL_RenderSetViewport(renderer, &rect);
}

WTexture::WTexture(WRenderer *renderer, WSurface *surface) {
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer->get(), surface->get());
    if (texture == nullptr) {
        throw std::runtime_error("failed to create texture from surface. error: " + std::string(SDL_GetError()));
    }
    this->texture = texture;
    this->width = surface->get()->w;
    this->height = surface->get()->h;
    this->renderer = renderer;
}

WTexture::WTexture(WRenderer *renderer, WSurface &&surface) {
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer->get(), surface.get());
    if (texture == nullptr) {
        throw std::runtime_error("failed to create texture from surface. error: " + std::string(SDL_GetError()));
    }
    this->texture = texture;
    this->width = surface.get()->w;
    this->height = surface.get()->h;
    this->renderer = renderer;
}

WTexture::~WTexture() {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
    }
}

void WTexture::render(SDL_Rect *src, SDL_Rect *dst) {
    SDL_RenderCopy(renderer->get(), texture, src, dst);
}

void WTexture::renderEx(SDL_Rect *src, SDL_Rect *dst, double angle, SDL_Point *point, SDL_RendererFlip flip) {
    SDL_RenderCopyEx(renderer->get(), texture, src, dst, angle, point, flip);
}


void WTexture::set_color_mod(int8_t r, int8_t g, int8_t b) {
    SDL_SetTextureColorMod(texture, r, g, b);
}

void WTexture::set_alpha_mod(uint8_t alpha) {
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(texture, alpha);
}

WBMPSurface::WBMPSurface(const std::string path) {
    SDL_Surface *surface = SDL_LoadBMP(path.c_str());
    if (surface == nullptr) {
        throw std::runtime_error("failed to load bmp: " + path + " error: " + std::string(SDL_GetError()));
    }
    this->surface = surface;
}

WBMPSurface::~WBMPSurface() {
    if (surface != nullptr) {
        SDL_FreeSurface(surface);
    }
}

void WBMPSurface::set_color_key(uint8_t r, uint8_t g, uint8_t b) {
    SDL_SetColorKey(surface, SDL_ENABLE, SDL_MapRGB(surface->format, r, g, b));
}

#ifdef SDL_IMG_ENABLED
IMG_Initializer::IMG_Initializer(int flags) {
    int img_flags = IMG_INIT_PNG;
    //only care about img_flag
    if (!(IMG_Init(img_flags) & img_flags)) {
        throw std::runtime_error("Failed to initialize SDL_image, error: " + std::string(SDL_GetError()));
    }
}

IMG_Initializer::~IMG_Initializer(){
    IMG_Quit();
}

WPNGSurface::WPNGSurface(std::string path) {
    SDL_Surface* loadSurface = IMG_Load(path.c_str());
    if (loadSurface == nullptr) {
        throw std::runtime_error("failed to load image " + path +  "error: " + std::string(IMG_GetError()));
    }         
    surface = loadSurface;
}

WPNGSurface::~WPNGSurface () {
    if (surface != nullptr) {
        SDL_FreeSurface(surface);
    }
}

void WPNGSurface::set_color_key(uint8_t r, uint8_t g, uint8_t b) {
    SDL_SetColorKey(surface, SDL_ENABLE, SDL_MapRGB(surface->format, r, g, b));
}
#endif 


#ifdef SDL_TTF_ENABLED
TTF_Initializer::TTF_Initializer() {
    if (TTF_Init() == -1) {
        throw std::runtime_error("Failed to initialize SDL_ttf, error: " + std::string(TTF_GetError()));
    }
}

TTF_Initializer::~TTF_Initializer() {
    TTF_Quit();
}

WTTFFont::WTTFFont(std::string path, uint size) {
    TTF_Font *font = TTF_OpenFont(path.c_str(), size);
    if (font == nullptr) {
        throw std::runtime_error("failed to open ttf font " + std::string(TTF_GetError()));
    }
    this->font = font;
}

void WTTFFont::set_size(uint size) {
    TTF_SetFontSize(font, size);
};

WTTFFont::~WTTFFont() {
    if (font != nullptr) {
        TTF_CloseFont(font);
    }
}

WTTFSurfaceSolid::WTTFSurfaceSolid(WTTFFont* font, std::string &&text, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderText_Solid(font->get(), text.c_str(), color);
    if (surface == nullptr) {
        throw std::runtime_error("failed to create ttf surface" + std::string(TTF_GetError()));
    } 
    this->surface = surface;
}

WTTFSurfaceSolid::WTTFSurfaceSolid(WTTFFont* font, std::string &text, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderText_Solid(font->get(), text.c_str(), color);
    if (surface == nullptr) {
        throw std::runtime_error("failed to create ttf surface" + std::string(TTF_GetError()));
    } 
    this->surface = surface;
}

WTTFSurfaceSolid::WTTFSurfaceSolid(WTTFFont* font, std::string &text, SDL_Color color, int wrap_length) {
    SDL_Surface *surface = TTF_RenderText_Solid_Wrapped(font->get(), text.c_str(), color, wrap_length);
    if (surface == nullptr) {
        throw std::runtime_error("failed to create ttf surface" + std::string(TTF_GetError()));
    } 
    this->surface = surface;
}

WTTFSurfaceSolid::~WTTFSurfaceSolid() {
    if (surface != nullptr) {
        SDL_FreeSurface(surface);
    }
}

WTTFSurfaceShaded::WTTFSurfaceShaded(WTTFFont* font, std::string &&text, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderText_Shaded(font->get(), text.c_str(), color, {255, 255, 255, 255});
    if (surface == nullptr) {
        throw std::runtime_error("failed to create ttf surface" + std::string(TTF_GetError()));
    } 
    this->surface = surface;
}

WTTFSurfaceShaded::WTTFSurfaceShaded(WTTFFont* font, std::string &text, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderText_Shaded(font->get(), text.c_str(), color, {255, 255, 255, 255});
    if (surface == nullptr) {
        throw std::runtime_error("failed to create ttf surface" + std::string(TTF_GetError()));
    } 
    this->surface = surface;
}

WTTFSurfaceShaded::WTTFSurfaceShaded(WTTFFont* font, std::string &text, SDL_Color color, int wrap_length) {
    SDL_Surface *surface = TTF_RenderText_Shaded_Wrapped(font->get(), text.c_str(), color, {255, 255, 255, 255},wrap_length);
    if (surface == nullptr) {
        throw std::runtime_error("failed to create ttf surface" + std::string(TTF_GetError()));
    } 
    this->surface = surface;
}

WTTFSurfaceShaded::~WTTFSurfaceShaded() {
    if (surface != nullptr) {
        SDL_FreeSurface(surface);
    }
}

WTTFSurfaceBlended::WTTFSurfaceBlended(WTTFFont* font, std::string &&text, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderText_Blended(font->get(), text.c_str(), color);
    if (surface == nullptr) {
        throw std::runtime_error("failed to create ttf surface" + std::string(TTF_GetError()));
    } 
    this->surface = surface;
}

WTTFSurfaceBlended::WTTFSurfaceBlended(WTTFFont* font, std::string &text, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderText_Blended(font->get(), text.c_str(), color);
    if (surface == nullptr) {
        throw std::runtime_error("failed to create ttf surface" + std::string(TTF_GetError()));
    } 
    this->surface = surface;
}

WTTFSurfaceBlended::WTTFSurfaceBlended(WTTFFont* font, std::string &text, SDL_Color color, int wrap_length) {
    SDL_Surface *surface = TTF_RenderText_Blended_Wrapped(font->get(), text.c_str(), color, wrap_length);
    if (surface == nullptr) {
        throw std::runtime_error("failed to create ttf surface" + std::string(TTF_GetError()));
    } 
    this->surface = surface;
}

WTTFSurfaceBlended::~WTTFSurfaceBlended() {
    if (surface != nullptr) {
        SDL_FreeSurface(surface);
    }
}
#endif

#ifdef SDL_MIX_ENABLED
Mix_Initializer::Mix_Initializer() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        throw std::runtime_error("failed to initialize Mixer, error: " + std::string(Mix_GetError()));
    }
}

Mix_Initializer::~Mix_Initializer() {
    Mix_Quit();
}

WMUS::WMUS(std::string path) {
    Mix_Music *music = Mix_LoadMUS(path.c_str());
    if (music == NULL) {
        throw std::runtime_error("failed to load music: " + path + ". error: " + std::string(Mix_GetError()));
    }
    this->music = music;
}

WMUS::~WMUS() {
    Mix_FreeMusic(music);
}

void WMUS::play(int loops) {
    Mix_PlayMusic(music, loops);
}

WWAV::WWAV(std::string path) {
    Mix_Chunk *chunk = Mix_LoadWAV(path.c_str());
    if (chunk == NULL) {
        throw std::runtime_error("failed to load wav: " + path + ". error: " + std::string(Mix_GetError()));
    }
    this->chunk = chunk;
}

WWAV::~WWAV() {
    Mix_FreeChunk(chunk);
}

void WWAV::play_channel(int channel, int loops) {
    Mix_PlayChannel(channel, chunk, loops);
}
#endif


