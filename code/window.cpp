#include "window.hpp"
#include <iostream>

/* Func: InitSDL
 * --------------------------------------
 * initializes SDL2 and SDL_image and reports eventual errors
 *
 * returns: true on success; false on failure
 */
bool Window::InitSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "[ERROR] Could not initialize SDL:\n" << SDL_GetError() << std::endl;
        return false;
    }
    
    int flags = IMG_INIT_PNG;
    if (!(IMG_Init(flags)) & flags) {
        std::cerr << "[ERROR] Could not initialize SDL_image:\n" << IMG_GetError() << std::endl;
        return false;
    }
    
    return true;
}


/* Func: SetWindowAttribs
 * -----------------------------------------
 */
void Window::SetContextAttribs(int v_major, int v_minor) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, v_major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, v_minor);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}


/* Constructor: Window
 * --------------------------------------
 * takes previously defined info (through functions) and creates window and context
 */
Window::Window(const char* t, int maj, int min, int x, int y, int w, int h) {
    if (!Window::InitSDL())
        exit(EXIT_FAILURE);
    SetContextAttribs(maj, min);
    
    this->window = SDL_CreateWindow(t, x, y, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "[ERROR] Could not create Window:\n" << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    
    this->context = SDL_GL_CreateContext(this->window);
    if (context == NULL) {
        std::cerr << "[ERROR] Could not create OpenGL Context:\n" << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
}








