#pragma once
#include "program_manager.hpp"
#include "camera_manager.hpp"
#include "texture.hpp"

namespace tsys {
    GLuint vao;
    ProgramMan &p = ProgramMan::instance();
    
    CameraMan *cam_man;
    TextureMan *tman;
    
    void Init();
    void InitBuffers();
    
    void Loop();
    
    bool input();
}
