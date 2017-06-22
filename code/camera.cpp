#include "camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
/* Constructor: Camera
 */
Camera::Camera(vec3 p, vec3 target, vec3 up) {
    this->U = normalize(up);
    this->N = normalize(target);
    this->V = normalize(cross(this->N, this->U));
    this->pos = p;
    
    this->SetMatrix();
}


mat4 Camera::getMatrix() {
    return (perspective(45.f, AR, 0.f, 100.f) * this->matrix);
}


Camera::~Camera() {
}

void Camera::SetMatrix() {
    this->matrix = lookAt(this->pos, this->pos + this->N, this->U);
}

void Camera::Right(float dist) {
    this->pos += dist * V;
    this->SetMatrix();
}

void Camera::Forward(float dist) {
    this->pos += dist * N;
    this->SetMatrix();
}
