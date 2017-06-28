#include "system.hpp"
#include "window.hpp"
#include "oglin.hpp"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define BUFFER_OFFSET(x) (GLvoid*)(x)

void tsys::Init() {
    /* creating window nad context initialization*/
    Window &win = Window::instance("test1", 4, 1, WIDTH, HEIGHT);
    /* initializing OpenGL program */
    p.NewProgram("light", "light.vert", "light.frag");
    p.NewProgram("cube", "shader.vert", "shader.frag");
    /* initializing camera manager */
    cam_man = new CameraMan();
    /* initializing texture manager */
    tman = new TextureMan();
    
    /* printing opengl version */
    win.printStats();
    
    InitBuffers();
    
    cam_man->Add("main", glm::vec3(0.0, 0.0, -3.0));
    cam_man->SetActive("main");
}


void tsys::Loop() {
    Window& win = Window::instance("test1");
    
    SDL_SetRelativeMouseMode(SDL_TRUE);
    
    glClearColor(0.0, 0.3, 0.5, 1.0);
    
    vaoman->BindVAO(10);
    while (input()) {
        glEnable(GL_DEPTH_TEST);
        p.SetActive("cube");
        glUniform3f(p.GetActiveUniformLocation("lights[0].pos"), 0.0, 1.0, 0.0);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        tman->Use("brick", 0, p.GetActiveUniformLocation("tex"));
        cam_man->SendUniformMatrix();
        cam_man->SendEyePosition();
        glm::mat4 model = glm::mat4(1.0);
        glUniformMatrix4fv(p.GetActiveUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        
        p.SetActive("light");
        cam_man->SendUniformMatrix();
        //model = glm::scale(model, glm::vec3(0.1));
        model = glm::translate(model, glm::vec3(0.0, 1.0, 0.0));
        model = glm::scale(model, glm::vec3(0.2));
        glUniformMatrix4fv(p.GetActiveUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        
        win.Swap();
    }
    vaoman->Unbind();
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
    size_t float_s = sizeof(GLfloat);
    
    GLfloat verts[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    
    GLuint *indices = new GLuint[36];
    for (int i = 0; i < 36; ++i) {
        indices[i] = i;
    }
    
    vaoman->NewVAO(10);
    {
        GLuint vbo, ebo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        
        glGenBuffers(1, &ebo);
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * float_s, BUFFER_OFFSET(0));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * float_s, BUFFER_OFFSET(3 * float_s));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * float_s, BUFFER_OFFSET(6 * float_s));
        glEnableVertexAttribArray(2);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 36, indices, GL_STATIC_DRAW);
    }
    
    tman->Add("container2.png", "brick");
    vaoman->Unbind();
}


