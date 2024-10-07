#include "SDL_utils/SDL_wrapper.h"
#include <SDL2/SDL_mixer.h>

SDL_Initializer::SDL_Initializer(int flags) {
    if (SDL_Init(flags) < 0) {
        throw std::runtime_error("Failed to initialize SDL, error: " + std::string(SDL_GetError()));
    }
} 

SDL_Initializer::~SDL_Initializer() {
    SDL_Quit();
}

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

TTF_Initializer::TTF_Initializer() {
    if (TTF_Init() == -1) {
        throw std::runtime_error("Failed to initialize SDL_ttf, error: " + std::string(TTF_GetError()));
    }
}

TTF_Initializer::~TTF_Initializer() {
    TTF_Quit();
}

Mix_Initializer::Mix_Initializer() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        throw std::runtime_error("failed to initialize Mixer, error: " + std::string(Mix_GetError()));
    }
}

Mix_Initializer::~Mix_Initializer() {
    Mix_Quit();
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

WPNGSurface::WPNGSurface(std::string path) {
    SDL_Surface* loadSurface = IMG_Load(path.c_str());
    if (loadSurface == nullptr) {
        throw std::runtime_error("failed to load image " + path +  "error: " + std::string(IMG_GetError()));
    }         
    surface = loadSurface;
}

void WPNGSurface::free() {
    if (surface != nullptr) {
        SDL_FreeSurface(surface);
    }
}

WPNGSurface::~WPNGSurface () {
    free();
}

WTexture::WTexture(SDL_Renderer *renderer, SDL_Surface *surface) {
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
        throw std::runtime_error("failed to create texture from surface. error: " + std::string(SDL_GetError()));
    }
    this->texture = texture;
    this->width = surface->w;
    this->height = surface->h;
}


WTexture::~WTexture() {
    free();
}

void WTexture::free() {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
    }
}

WRenderer::WRenderer(SDL_Window * window, int index, uint32_t flags) {
    SDL_Renderer *renderer = SDL_CreateRenderer(window, index, flags);
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

WTTFFont::WTTFFont(std::string path, int size) {
    TTF_Font *font = TTF_OpenFont(path.c_str(), size);
    if (font == nullptr) {
        throw std::runtime_error("failed to open ttf font " + std::string(TTF_GetError()));
    }
    this->font = font;
}

WTTFFont::~WTTFFont() {
    if (font != nullptr) {
        TTF_CloseFont(font);
    }
}

WTTFSurface::WTTFSurface(TTF_Font* font, std::string text, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (surface == nullptr) {
        throw std::runtime_error("failed to create ttf surface" + std::string(TTF_GetError()));
    } 
    this->surface = surface;
}

WTTFSurface::~WTTFSurface() {
    if (surface != nullptr) {
        SDL_FreeSurface(surface);
    }
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


