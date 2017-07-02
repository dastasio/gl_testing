#pragma once
#include "program_manager.hpp"
#include "camera_manager.hpp"
#include "vao_manager.hpp"
#include "texture.hpp"
#include "scene.hpp"

namespace tsys {
    ProgramMan &p = ProgramMan::instance();
    VAOMan *vaoman = new VAOMan();
    Scene *sc;
    
    CameraMan *cam_man;
    TextureMan *tman;
    
    void Init();
    void InitBuffers();
    
    void Loop();
    
    bool input();
}
