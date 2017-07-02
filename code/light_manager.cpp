#include "light_manager.hpp"

LightMan::LightMan() {
    ProgramMan &pman = ProgramMan::instance();
    
    this->UniformArrayLocation = pman.GetActiveUniformLocation("lights");
    this->UniformArraySizeLocation = pman.GetActiveUniformLocation("N_LIGHTS");
}

int LightMan::NewLight(glm::vec3 p, glm::vec3 c) {
    
    this->lights.push_back(Light(p.x, p.y, p.z, c.r, c.g, c.b));
    return lights.size() - 1;
}

void LightMan::EraseLight(int light_id) {
    this->lights.erase(this->lights.begin() + light_id);
}


void LightMan::CalculateLighting() {
    glUniform3fv(UniformArrayLocation, lights.size() * 2, &lights.data()->px);
    glUniform1i(UniformArraySizeLocation, this->lights.size());
}


LightMan::~LightMan() {
    delete &this->UniformArraySizeLocation;
    delete &this->UniformArrayLocation;
    lights.clear();
    delete &this->lights;
}
