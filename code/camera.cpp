#include "camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

using namespace glm;
/* Constructor: Camera
 * --------------------------------------------
 * Constructs a new camera with given attributes
 *
 * p: position of the new camera
 * target: direction in which the camera will look at
 * up: up direction for the camera (should,in most cases, be X:0,Y:1,Z:0)
 */
Camera::Camera(vec3 p, vec3 target, vec3 up) {
    this->U = normalize(up);
    this->N = normalize(target);
    this->V = normalize(cross(this->N, this->U));
    this->pos = p;
    
    this->SetMatrix();
}

/* Destructor: Camera
 * ------------------------------------
 * Deletes all members
 */
Camera::~Camera() {
    delete &this->U;
    delete &this->V;
    delete &this->N;
    delete &this->pos;
    delete &this->matrix;
}


/* Getter: Camera space converter matrix
 * --------------------------------------------------
 * returns: matrix that includes camera space and perspective transform
 */
mat4 Camera::getCamSpace() {
    return this->matrix;
}

/* Getter: Projection Matrix
 * --------------------------------------------------
 * returns: matrix that includes camera space and perspective transform
 */
mat4 Camera::getProjection() {
    return perspective(45.f, AR, 0.3f, 100.f);
}

/* Getter: Position
 * --------------------------------------------------
 * returns: camera position in vec3
 */
vec3 Camera::getPosition() {
    return this->pos;
}

/* Func: SetMatrix
 * --------------------------------------------------
 * Reinitializes matrix with currently saved vectors
 */
void Camera::SetMatrix() {
    this->matrix = lookAt(this->pos, this->pos + this->N, this->U);
}


/* Func: Up
 * ------------------------------------------------
 * Moves camera along local Y axis
 *
 * dist: distance to travel up or down(if negative)
 */
void Camera::Up(float dist) {
    this->pos.y += dist;
    this->SetMatrix();
}

/* Func: Right
 * ------------------------------------------------
 * Moves camera along local X axis
 *
 * dist: distance to travel right or left(if negative)
 */
void Camera::Right(float dist) {
    this->pos += dist * vec3(V.x, 0.0, V.z);
    this->SetMatrix();
}


/* Func: Forward
 * -------------------------------------------------
 * Moves camera along local Z axis
 *
 * dist: distance to travel forward or backward(if negative)
 */
void Camera::Forward(float dist) {
    this->pos += dist * vec3(N.x, 0.0, N.z);
    this->SetMatrix();
}


/* Func: Pitch
 * -------------------------------------------------
 * Rotates camera on its pitch angle (around local X axis)
 *
 * angle: angle of rotation
 */
void Camera::Pitch(float angle) {
    bool up_restriction = N.y >= 0.98 && angle > 0;
    bool down_restriction = N.y <= -0.98 && angle < 0;
    if (up_restriction || down_restriction)
        angle = 0;
    this->N = normalize(rotate(N, angle, this->V));
    SetMatrix();
}


/* Func: Yaw
 * --------------------------------------------------
 * Rotates camera on its yaw angle (around local Y axis)
 */
void Camera::Yaw(float angle) {
    this->N = normalize(rotate(this->N, angle, vec3(0.0, 1.0, 0.0)));
    this->V = normalize(cross(N, U));
    SetMatrix();
}
