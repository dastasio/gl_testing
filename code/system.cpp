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
    p.NewProgram("shadowMapping", "assets/shadow.vert", "assets/shadow.frag");
    p.NewProgram("main", "assets/shader.vert", "assets/shader.frag");
    /* initializing camera manager */
    cam_man = new CameraMan();
    
    /* printing opengl version */
    win.printStats();
    
    InitBuffers();
    
    cam_man->Add("main", vec3(0.0, 0.2, -3.0));
    cam_man->SetActive("main");
}

void tsys::DrawShadows(glm::mat4 LightSpaceMatrix) {
    glUniformMatrix4fv(p.GetActiveUniformLocation("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(LightSpaceMatrix));
    
    sc->Draw(GL_FALSE, glm::vec3(1.0));
}

void tsys::Loop() {
    Window& win = Window::instance("");
    LightMan* lman = new LightMan();
//    lman->NewPointLight(glm::vec3(-1.2, 1.9, 1.5), glm::vec3(1.0));
    lman->NewLightShadowMapped(glm::vec3(-0.2, -1.0, -0.3), glm::vec3(1.0, 0.9, 0.5));
    
    SDL_SetRelativeMouseMode(SDL_TRUE);
    
    glClearColor(0.05, 0.05, 0.05, 1.0);
    
    TextureMan *tman = new TextureMan();
    glm::mat4 lightSpaceMatrix;
    
    while (input()) {
        vaoman.BindVAO(10);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        lman->CalculateLighting(); // used to prevent light arrays from being deleted
        
        p.SetActive("shadowMapping");
        GLuint shadowMap = lman->MapShadows(tsys::DrawShadows, lightSpaceMatrix);
        
        p.SetActive("main");
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cam_man->SendUniformMatrix();
        cam_man->SendEyePosition();
        lman->CalculateLighting();
        tman->ActiveShadowMap(p.GetActiveUniformLocation("shadowMap"), shadowMap);
        glUniformMatrix4fv(p.GetActiveUniformLocation("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
        glUniform1d(p.GetActiveUniformLocation("gamma"), 2.2);
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
    
    /* getting mouse movement */
    int dx, dy;
    SDL_GetRelativeMouseState(&dx, &dy);
    
    cam_man->RotateCamera(ROT_PITCH, dy * rotation_speed);
    cam_man->RotateCamera(ROT_YAW, dx * rotation_speed);
    
    return true;
}





