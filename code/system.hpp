#pragma once
#include "shaders.hpp"
#include "camera.hpp"

namespace tsys {
    GLuint vao, vbo;
    Program *p;
    Camera cam(glm::vec3(0.0, 0.0, -3.0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 1.0, 0.0));
    
    void Init();
    void InitBuffers();
    
    void Loop();
    
    bool input();
}
