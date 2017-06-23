#pragma once
#include "shaders.hpp"
#include "camera_manager.hpp"

namespace tsys {
    GLuint vao, vbo;
    Program *p;
    CameraMan *cam_man;
    
    void Init();
    void InitBuffers();
    
    void Loop();
    
    bool input();
}
