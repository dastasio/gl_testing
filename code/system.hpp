#pragma once
#include "program_manager.hpp"
#include "camera_manager.hpp"
#include "vao_manager.hpp"
#include "texture.hpp"
#include "scene.hpp"

namespace tsys {
    ProgramMan &p = ProgramMan::instance();
    VAOMan &vaoman = VAOMan::instance();
    Scene *sc;
    
    CameraMan *cam_man;
    
    void Init();
    void InitBuffers();
    
    void Loop();
    
    bool input();
}
