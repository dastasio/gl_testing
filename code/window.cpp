#include "window.hpp"
#include <iostream>
#include "oglin.hpp"

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
    
    int flags = IMG_INIT_PNG | IMG_INIT_JPG;
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
//    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
//    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
}


/* Constructor: Window
 * --------------------------------------
 * takes previously defined info (through functions) and creates window and context
 */
Window::Window(const char* t, int x, int y, int w, int h, int maj, int min) {
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
    else {
        SDL_GL_MakeCurrent(this->window, this->context);
        
		if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
			std::cerr << "[ERROR] Could not initialize glad!" << std::endl;
			exit(EXIT_FAILURE);
		}

        glViewport(0, 0, w, h);
        this->width = w;
        this->height = h;
    }
}



void Window::printStats() {
    std::cout << "OpenGL version :" << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version   :" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}


void Window::Swap() {
    SDL_GL_SwapWindow(this->window);
}



GLuint Window::NewFramebuffer() {
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    return fbo;
}


GLuint Window::InitFramebuffer(GLuint fbo) {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    /* allocating memory for color */
    GLuint color;
    glGenTextures(1, &color);
    glBindTexture(GL_TEXTURE_2D, color);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    /* attaching color memory to framebuffer */
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);
    
    /* allocating memory for depth and stencil*/
    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    /* attaching depth/stencil memory to framebuffer */
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "[ERROR] Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return color;
}



