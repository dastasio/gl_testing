#include "system.hpp"
#include "window.hpp"
#include "oglin.hpp"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#define BUFFER_OFFSET(x) (GLvoid*)(x)

void tsys::Init() {
    /* creating window */
    Window &win = Window::instance("test1", 4, 1, WIDTH, HEIGHT);
    p = new Program("shader.vert", "shader.frag");
    
    win.printStats();
    
    InitBuffers();
}


void tsys::Loop() {
    Window& win = Window::instance("test1");
    SDL_Window* window = win.getWindow();
    p->compile();
    p->use();
    GLint uniformCamLoc = glGetUniformLocation(p->getProgram(), "camera");
    
    
    
    glClearColor(0.0, 0.3, 0.5, 1.0);
    
    while (input()) {
        glClear(GL_COLOR_BUFFER_BIT);
        glUniformMatrix4fv(uniformCamLoc, 1, GL_FALSE, glm::value_ptr(cam.getMatrix()));
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        SDL_GL_SwapWindow(window);
    }
}



bool tsys::input() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            return false;
    }
    
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    
    if (state[SDL_SCANCODE_ESCAPE]) {
        return false;
    }
    if (state[SDL_SCANCODE_D]) {
        cam.Right(0.2);
    }
    if (state[SDL_SCANCODE_A]) {
        cam.Right(-0.2);
    }
    if (state[SDL_SCANCODE_W]) {
        cam.Forward(0.2);
    }
    if (state[SDL_SCANCODE_S]) {
        cam.Forward(-0.2);
    }
    
    return true;
}





void tsys::InitBuffers() {
    GLfloat verts[] = {
        -0.75, -0.75, 1.0,
         0.75, -0.75, 1.0,
         0.00,  0.75, 1.0
    };
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);
}


