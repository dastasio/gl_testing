#include "system.hpp"
#include "window.hpp"
#include "oglin.hpp"
#include "light_manager.hpp"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define BUFFER_OFFSET(x) (GLvoid*)(x)

using namespace glm;
void tsys::Init() {
    /* creating window nad context initialization*/
    Window &win = Window::instance("test1", 4, 1, WIDTH, HEIGHT);
    /* initializing OpenGL program */
    p.NewProgram("main", "assets/shader.vert", "assets/shader.frag", "assets/shader.geom");
    /* initializing camera manager */
    cam_man = new CameraMan();
    
    /* printing opengl version */
    win.printStats();
    
    InitBuffers();
    
    cam_man->Add("main", vec3(0.0, 0.0, -3.0));
    cam_man->SetActive("main");
}


void tsys::Loop() {
    Window& win = Window::instance("");
//    LightMan* lman = new LightMan();
//    lman->NewLight(glm::vec3(0.0, 2.0, 0.0), glm::vec3(1.0, 0.9, 0.0));
    
    SDL_SetRelativeMouseMode(SDL_TRUE);
    
    glClearColor(0.05, 0.05, 0.05, 1.0);
    
    p.SetActive("main");
    while (input()) {
        vaoman.BindVAO(10);
        glEnable(GL_DEPTH_TEST);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        cam_man->SendUniformMatrix();
        glUniform1d(p.GetActiveUniformLocation("anim_time"), glm::sin(GLdouble(SDL_GetTicks() / 1000.f)));
        sc->Draw(GL_TRUE, glm::vec3(1.0));
        
//        p.SetActive("lights");
//        lman->RenderLights();
        
        win.Swap();
    }
    vaoman.Unbind();
}


void tsys::InitBuffers() {
    vaoman.NewVAO(10);
    {
        sc = new Scene("assets/nanosuit/nanosuit.obj");
    }
    vaoman.Unbind();
}

bool tsys::input() {
    static const Uint8 *state = SDL_GetKeyboardState(NULL);
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            return false;
    }
    
    double rotation_speed = -1.5 * M_PI / WIDTH;
    double movement_speed = 0.07;
    
    if (state[SDL_SCANCODE_ESCAPE]) {
        return false;
    }
    if (state[SDL_SCANCODE_RCTRL]) {
        movement_speed = 0.005;
    }
    else {
        movement_speed = 0.07;
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





