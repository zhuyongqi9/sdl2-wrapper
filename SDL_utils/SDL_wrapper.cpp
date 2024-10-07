#include "SDL_utils/SDL_wrapper.h"
#include <SDL2/SDL_render.h>
#include <stdexcept>

SDL_Initializer::SDL_Initializer(int flags) {
    if (flags & W_SDL_INIT_VIDEO) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            throw std::runtime_error("Failed to initialize SDL, error: " + std::string(SDL_GetError()));
        }
    } else if (flags & W_IMG_INIT_PNG) {
        int img_flags = IMG_INIT_PNG;
        //only care about img_flag
        if (!(IMG_Init(img_flags) & img_flags)) {
            throw std::runtime_error("Failed to initialize SDL, error: " + std::string(SDL_GetError()));
        }
    }
    this->flags = flags;
} 

SDL_Initializer::~SDL_Initializer() {
    if (flags & W_SDL_INIT_VIDEO) {
        SDL_Quit();
    } else if (flags & W_SDL_INIT_VIDEO) {
        IMG_Quit();
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

WSurface::WSurface(std::string path) {
    SDL_Surface* loadSurface = IMG_Load(path.c_str());
    if (loadSurface == nullptr) {
        throw std::runtime_error("failed to load image " + path +  "error: " + std::string(IMG_GetError()));
    }         
    surface = loadSurface;
}

void WSurface::free() {
    if (surface != nullptr) {
        SDL_FreeSurface(surface);
    }
}

WSurface::~WSurface () {
    free();
}

WTexture::WTexture(std::string path, SDL_Renderer *render) {
    try {
        WSurface loadSurface = WSurface(path);

        SDL_Texture* newTexture = NULL;
        if ((newTexture = SDL_CreateTextureFromSurface(render, loadSurface.get())) == nullptr) {
            throw std::runtime_error("failed to create surface, error: " + std::string(SDL_GetError()));
        }
        texture = newTexture;
        width = loadSurface.get()->w;
        height = loadSurface.get()->h;
    } catch (const std::exception &e) {
        throw e;
    }
}

WTexture::WTexture(std::string path, SDL_Renderer *render, WRGB rgb) {
    try {
        WSurface loadSurface = WSurface(path);
        SDL_SetColorKey(loadSurface.get(), SDL_TRUE, SDL_MapRGB(loadSurface.get()->format, rgb.r, rgb.g,  rgb.b));

        SDL_Texture* newTexture = NULL;
        if ((newTexture = SDL_CreateTextureFromSurface(render, loadSurface.get())) == nullptr) {
            throw std::runtime_error("failed to create surface, error: " + std::string(SDL_GetError()));
        }
        texture = newTexture;
        width = loadSurface.get()->w;
        height = loadSurface.get()->h;
    } catch (const std::exception &e) {
        throw e;
    }

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