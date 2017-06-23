#include "system.hpp"
#include "window.hpp"
#include "oglin.hpp"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#define BUFFER_OFFSET(x) (GLvoid*)(x)

void tsys::Init() {
    /* creating window nad context initialization*/
    Window &win = Window::instance("test1", 4, 1, WIDTH, HEIGHT);
    /* initializing OpenGL program */
    p = new Program("shader.vert", "shader.frag");
    p->compile();
    /* initializing camera manager */
    cam_man = new CameraMan(p);
    
    win.printStats();
    
    InitBuffers();
    
    cam_man->Add("main", glm::vec3(0.0, 0.0, -3.0));
    cam_man->SetActive("main");
}


void tsys::Loop() {
    Window& win = Window::instance("test1");
    p->use();
    
    SDL_SetRelativeMouseMode(SDL_TRUE);
    
    glClearColor(0.0, 0.3, 0.5, 1.0);
    
    while (input()) {
        glClear(GL_COLOR_BUFFER_BIT);
        cam_man->SendUniformMatrix();
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        win.Swap();
    }
}



bool tsys::input() {
    static const Uint8 *state = SDL_GetKeyboardState(NULL);
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            return false;
    }
    
    double rotation_speed = -2 * M_PI / WIDTH;
    double movement_speed = 0.2;
    
    if (state[SDL_SCANCODE_ESCAPE]) {
        return false;
    }
    if (state[SDL_SCANCODE_D]) {
        cam_man->MoveCamera(MOVE_RIGHT, movement_speed);
    }
    if (state[SDL_SCANCODE_A]) {
        cam_man->MoveCamera(MOVE_RIGHT, -movement_speed);
    }
    if (state[SDL_SCANCODE_W]) {
        cam_man->MoveCamera(MOVE_FORWARD, movement_speed);
    }
    if (state[SDL_SCANCODE_S]) {
        cam_man->MoveCamera(MOVE_FORWARD, -movement_speed);
    }
    if (state[SDL_SCANCODE_SPACE]) {
        cam_man->MoveCamera(MOVE_UP, movement_speed);
    }
    if (state[SDL_SCANCODE_LSHIFT]) {
        cam_man->MoveCamera(MOVE_UP, -movement_speed);
    }
    
    /* getting mouse movement */
    int dx, dy;
    SDL_GetRelativeMouseState(&dx, &dy);
    
    cam_man->RotateCamera(ROT_PITCH, dy * rotation_speed);
    cam_man->RotateCamera(ROT_YAW, dx * rotation_speed);
    
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


