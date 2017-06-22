#include "camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

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
    this->pos += dist * vec3(V.x, 0.0, V.z);
    this->SetMatrix();
}

void Camera::Forward(float dist) {
    this->pos += dist * vec3(N.x, 0.0, N.z);
    this->SetMatrix();
}


void Camera::Pitch(float angle) {
    this->N = normalize(rotate(N, angle, this->V));
    
    SetMatrix();
}


void Camera::Yaw(float angle) {
    this->N = normalize(rotate(this->N, angle, vec3(0.0, 1.0, 0.0)));
    this->V = normalize(cross(N, U));
    
    SetMatrix();
}
