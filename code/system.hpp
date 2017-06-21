#pragma once
#include "shaders.hpp"

namespace tsys {
    GLuint vao, vbo;
    Program *p;
    
    void Init();
    void InitBuffers();
    
    void Loop();
    
    bool input();
}
