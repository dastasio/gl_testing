#pragma once
#include <glm/glm.hpp>
#define WIDTH 1024.f
#define HEIGHT 720.f
#define AR WIDTH/HEIGHT

class Camera {
public:
    Camera(glm::vec3 p, glm::vec3 target, glm::vec3 up);
    ~Camera();
    
    glm::mat4 getMatrix();
    
    void Right(float dist);
    void Forward(float dist);
    void Pitch(float angle);
    void Yaw(float angle);
private:
    void SetMatrix();
    
    glm::vec3 U, V, N;
    glm::vec3 pos;
    
    glm::mat4 matrix;
};
