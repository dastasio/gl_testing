#include "system.hpp"
#include "window.hpp"
#include "oglin.hpp"
#include <iostream>
void tsys::Init() {
    /* creating window */
    Window &win = Window::instance("test1");
    p = new Program("shader.vert", "shader.frag");
    
    win.printStats();
}


void tsys::Loop() {
    Window& win = Window::instance("test1");
    SDL_Window* window = win.getWindow();
    p->compile();
    p->use();
    
    glClearColor(0.0, 0.3, 0.5, 1.0);
    
    while (input()) {
        glClear(GL_COLOR_BUFFER_BIT);
        
        SDL_GL_SwapWindow(window);
        
        SDL_Delay(200);
    }
}



bool tsys::input() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            return false;
    }
    
    return true;
}
