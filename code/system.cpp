#include "system.hpp"
#include "window.hpp"
#include "oglin.hpp"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define BUFFER_OFFSET(x) (GLvoid*)(x)

GLboolean depthVisual = GL_FALSE;

ProgramMan &tsys::p = ProgramMan::instance();
VAOMan &tsys::vaoman = VAOMan::instance();
LightMan* tsys::lman;
Scene *tsys::sc;

CameraMan *tsys::cam_man;

using namespace glm;
void tsys::Init() {
    /* creating window nad context initialization*/
    Window &win = Window::instance("test1", 4, 1, WIDTH, HEIGHT);
    /* initializing OpenGL program */
    p.NewProgram("shadowMapping", "assets/shadow.vert", "assets/shadow.frag", "assets/shadow.geom");
    p.NewProgram("main", "assets/shader.vert", "assets/shader.frag");
    /* initializing camera manager */
    cam_man = new CameraMan();
    lman = new LightMan();
    
    /* printing opengl version */
    win.printStats();
    
    InitBuffers();
    
    cam_man->Add("main", vec3(0.0, 0.2, -3.0));
    cam_man->SetActive("main");
}

void tsys::DrawShadows() {
    sc->Draw(GL_FALSE, glm::vec3(1.0));
}

void tsys::Loop() {
    Window& win = Window::instance("");
//    lman->NewPointLight(glm::vec3(-1.2, 1.9, 1.5), glm::vec3(1.0));
    lman->NewPointShadowMap(glm::vec3(0.0, 1.0, 2.0), glm::vec3(1.0));
    
    SDL_SetRelativeMouseMode(SDL_TRUE);
    
    glClearColor(0.05, 0.05, 0.05, 1.0);
    
    while (input()) {
        vaoman.BindVAO(10);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_MULTISAMPLE);
//        lman->CalculateLighting(); // used to prevent light arrays from being deleted
        
        p.SetActive("shadowMapping");
        glUniform1f(p.GetActiveUniformLocation("far_plane"), 25.f);
        GLuint cubeDepth = lman->MapPointShadows(tsys::DrawShadows);
        
        p.SetActive("main");
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cam_man->SendUniformMatrix();
        cam_man->SendEyePosition();
        lman->CalculateLighting();
        glActiveTexture(GL_TEXTURE18);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeDepth);
        glUniform1i(p.GetActiveUniformLocation("shadowMap"), 18);
        glUniform1f(p.GetActiveUniformLocation("far_plane"), 25.f);
        glUniform3fv(p.GetActiveUniformLocation("lightpos"), 1, glm::value_ptr(lman->getShadowPos()));
        glUniform1d(p.GetActiveUniformLocation("gamma"), 2.2);
        glUniform1i(p.GetActiveUniformLocation("depthVisual"), depthVisual);
        sc->Draw(GL_TRUE, glm::vec3(1.0));
        
        p.SetActive("lights");
        cam_man->SendUniformMatrix();
        lman->RenderLights();
        
        win.Swap();
    }
    vaoman.Unbind();
}


void tsys::InitBuffers() {
    vaoman.NewVAO(10);
    {
        sc = new Scene("assets/shadow_test.fbx");
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
    if (state[SDL_SCANCODE_UP])
        lman->MoveLight(0.1f * glm::vec3(0.0, 1.0, 0.0));
    if (state[SDL_SCANCODE_DOWN])
        lman->MoveLight(0.1f * glm::vec3(0.0, -1.0, 0.0));
    if (state[SDL_SCANCODE_J])
        lman->MoveLight(0.1f * glm::vec3(0.0, 0.0, -1.0));
    if (state[SDL_SCANCODE_I])
        lman->MoveLight(0.1f * glm::vec3(-1.0, 0.0, 0.0));
    if (state[SDL_SCANCODE_L])
        lman->MoveLight(0.1f * glm::vec3(0.0, 0.0, 1.0));
    if (state[SDL_SCANCODE_K])
        lman->MoveLight(0.1f * glm::vec3(1.0, 0.0, 0.0));
    if (state[SDL_SCANCODE_R]) {
        depthVisual = !depthVisual;
        SDL_Delay(100);
    }

    
    /* getting mouse movement */
    int dx, dy;
    SDL_GetRelativeMouseState(&dx, &dy);
    
    cam_man->RotateCamera(ROT_PITCH, dy * rotation_speed);
    cam_man->RotateCamera(ROT_YAW, dx * rotation_speed);
    
    return true;
}





