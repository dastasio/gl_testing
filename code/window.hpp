#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "oglin.hpp"
#define PUNDEF SDL_WINDOWPOS_UNDEFINED


class Window {
public:
    static Window& instance(const char* t,
                            int v_maj = 4, int v_min = 1,
                            int w = 1024, int h = 720,
                            int x = PUNDEF, int y = PUNDEF) {
        static Window* inst = new Window(t,
                                         x, y,
                                         w, h,
                                         v_maj, v_min);
        return *inst;
    }
    
    SDL_Window* getWindow() {
        return this->window;
    }
    
    void Swap();
    
    void printStats();
    
    GLuint NewFramebuffer();
    GLuint InitFramebuffer(GLuint fbo);
private:
    Window(const char* t,
           int x, int y,
           int w, int h,
           int maj, int min);
    ~Window();
    
    GLint width, height;
    
    SDL_Window* window;
    SDL_GLContext context;
    
    bool CreateContext();
    void SetContextAttribs(int v_major, int v_minor);
    
    static bool InitSDL();
};
