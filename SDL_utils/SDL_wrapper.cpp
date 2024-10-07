#include "SDL_utils/SDL_wrapper.h"
SDL_Initializer::SDL_Initializer(int flags) {
    if (flags & W_SDL_INIT_VIDEO) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            throw std::runtime_error("Failed to initialize SDL, error: " + std::string(SDL_GetError()));
        }
    } 
    
    if (flags & W_IMG_INIT_PNG) {
        int img_flags = IMG_INIT_PNG;
        //only care about img_flag
        if (!(IMG_Init(img_flags) & img_flags)) {
            throw std::runtime_error("Failed to initialize SDL_image, error: " + std::string(SDL_GetError()));
        }
    } 
    
    if (flags & W_TTF_INIT) {
        if (TTF_Init() == -1) {
            throw std::runtime_error("Failed to initialize SDL_ttf, error: " + std::string(TTF_GetError()));
        }
    }
    this->flags = flags;
} 

SDL_Initializer::~SDL_Initializer() {
    if (flags & W_SDL_INIT_VIDEO) {
        SDL_Quit();
    } 
    
    if (flags & W_SDL_INIT_VIDEO) {
        IMG_Quit();
    }

    if (flags & W_TTF_INIT) {
        TTF_Quit();
    }
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
        throw std::runtime_error("failed to create texture from surface");
    }
    this->texture = texture;
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

