#pragma once
#include "program_manager.hpp"
#include <glm/glm.hpp>
#include <vector>

struct Light {
    GLfloat px, py, pz;
    GLfloat cr, cg, cb;
    
    Light(GLfloat x, GLfloat y, GLfloat z,
          GLfloat r, GLfloat g, GLfloat b) :
    px(x), py(y), pz(z), cr(r), cg(g), cb(b) {}
};

class LightMan {
public:
    LightMan();
    ~LightMan();
    
    int NewLight(glm::vec3 p, glm::vec3 c);
    void EraseLight(int light_id);
    
    void CalculateLighting();
private:
    std::vector<Light> lights;
    
    GLint UniformArrayLocation;
    GLint UniformArraySizeLocation;
};
