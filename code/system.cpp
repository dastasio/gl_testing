#include "system.hpp"
#include "window.hpp"
#include "oglin.hpp"
#include <iostream>
#define BUFFER_OFFSET(x) (GLvoid*)(x)

void tsys::Init() {
    /* creating window */
    Window &win = Window::instance("test1");
    p = new Program("shader.vert", "shader.frag");
    
    win.printStats();
    
    InitBuffers();
}


void tsys::Loop() {
    Window& win = Window::instance("test1");
    SDL_Window* window = win.getWindow();
    p->compile();
    p->use();
    
    glClearColor(0.0, 0.3, 0.5, 1.0);
    
    while (input()) {
        glClear(GL_COLOR_BUFFER_BIT);
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
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





void tsys::InitBuffers() {
    GLfloat verts[] = {
        -0.75, -0.75,
         0.75, -0.75,
         0.00,  0.75
    };
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);
}


