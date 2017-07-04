#pragma once
#include "program_manager.hpp"
#include "vao_manager.hpp"
#include <glm/glm.hpp>
#include <vector>

struct Light {
    glm::vec3 position;
    glm::vec3 color;
    
    Light(GLfloat x, GLfloat y, GLfloat z,
          GLfloat r, GLfloat g, GLfloat b) :
    position(x, y, z), color(r, g, b) {}
    
    Light(glm::vec3 p, glm::vec3 c) :
    position(p), color(c) {}
};

class LightMan {
public:
    LightMan();
    ~LightMan();
    
    int NewLight(glm::vec3 p, glm::vec3 c);
    void EraseLight(int light_id);
    
    void CalculateLighting();
    void RenderLights();
private:
    std::vector<Light> lights;
    
    GLint UniformArrayLocation;
    GLint UniformArraySizeLocation;
    GLint UniformColorLocation;
    
    static void InitVAO();
};
