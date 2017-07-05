#pragma once
#include "program_manager.hpp"
#include "vao_manager.hpp"
#include <glm/glm.hpp>
#include <vector>

struct Light {
    typedef glm::vec3 vec3;
    vec3 position;
    vec3 c_amb;
    vec3 c_diff;
    vec3 c_spec;
    
    Light(vec3 p, vec3 ca, vec3 cd, vec3 cs) :
    position(p), c_amb(ca), c_diff(cd), c_spec(cs) {}
    
    Light(vec3 p, vec3 c) :
    position(p), c_amb(c * 0.07f), c_diff(c), c_spec(c) {}
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
