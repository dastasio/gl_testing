#pragma once
#include "shaders.hpp"
#include "camera_manager.hpp"
#include "texture.hpp"

namespace tsys {
    GLuint vao, vbo, ebo;
    Program *p;
    CameraMan *cam_man;
    TextureMan *tman;
    
    void Init();
    void InitBuffers();
    
    void Loop();
    
    bool input();
}
