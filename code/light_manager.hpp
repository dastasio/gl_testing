#pragma once
#include "program_manager.hpp"
#include "vao_manager.hpp"
#include <glm/glm.hpp>
#include <vector>
#define SHADOW_MAP_WH 1024

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
    
    void NewPointLight(glm::vec3 p, glm::vec3 c);
    void NewDirLight(glm::vec3 dir, glm::vec3 c);
    void NewDirShadowMap(glm::vec3 dir, glm::vec3 c);
    void NewPointShadowMap(glm::vec3 p, glm::vec3 c);
    
    void CalculateLighting();
    GLuint MapDirShadows(void (*drawScene)(glm::mat4), glm::mat4&);
    GLuint MapPointShadows(void (*drawScene)());
    void RenderLights();
    
    void MoveLight(glm::vec3 dir);
    glm::vec3 getShadowPos() {
        return point_shadow->position;
    }
private:
    std::vector<Light> point_lights;
    std::vector<Light> dir_lights;
    std::vector<glm::mat4> lightSpaces;
    Light *directional_shadow;
    Light *point_shadow;
    GLuint shadow_map2D, shadow_cubemap,
    shadow2D_fb, shadowcube_fb;
    
    GLint UniformArrayLocation_point;
    GLint UniformArrayLocation_dir;
    GLint UniformArraySizeLocation_point;
    GLint UniformArraySizeLocation_dir;
    GLint UniformColorLocation;
    
    static void InitVAO();
};
