#pragma once
#include <SDL.h>
#include <SDL_image.h>
#define PUNDEF SDL_WINDOWPOS_UNDEFINED


class Window {
public:
    static Window& instance(const char* t,
                            int v_maj = 4, int v_min = 1,
                            int x = PUNDEF, int y = PUNDEF,
                            int w = 1024, int h = 720) {
        static Window* inst = new Window(t, x, y, w, h, v_maj, v_min);
        return *inst;
    }
    
    SDL_Window* getWindow() {
        return this->window;
    }
private:
    Window(const char* t, int x, int y, int w, int h, int maj, int min);
    ~Window();
    
    SDL_Window* window;
    SDL_GLContext context;
    
    bool CreateContext();
    void SetContextAttribs(int v_major, int v_minor);
    
    static bool InitSDL();
};
