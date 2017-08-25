#pragma once
#include "program_manager.hpp"
#include "camera_manager.hpp"
#include "light_manager.hpp"
#include "vao_manager.hpp"
#include "texture.hpp"
#include "scene.hpp"
#define SHOW_WARNINGS

namespace tsys {
    extern ProgramMan &p;
	extern VAOMan &vaoman;
    extern LightMan* lman;
    extern Scene *sc;
     
    extern CameraMan *cam_man;
    
    void Init();
    void InitBuffers();
    
    void DrawShadows();
    void Loop();
    
    bool input();
}
