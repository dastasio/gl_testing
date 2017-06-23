#include "camera_manager.hpp"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#define VEC_UP glm::vec3(0.0, 1.0, 0.0)
#define VEC_TARGET glm::vec3(0.0, 0.0, 1.0)

CameraMan::CameraMan(Program *p) {
    this->prog = p;
    
    this->UniformCamLoc = prog->getUniformLocation("camera");
}

CameraMan::~CameraMan() {
    /* destroying all cameras */
    for (auto it = cams.begin(); it != cams.end(); ++it) {
        delete it->second;
    }
    
    delete &this->cams;
    delete &this->UniformCamLoc;
    delete active_cam;
    delete active_name;
    std::free(prog);
}


/* Func: Add
 * ------------------------------------------------------
 * Adds new camera to internal map with given name and position
 *
 * name: unique ID that identifies the new camera
 * pos: position where the new camera will be created
 *
 * s.e.: prints warning if a camera with same name already exists
 */
void CameraMan::Add(const char *name, glm::vec3 pos) {
    Camera* newCam = new Camera(pos, VEC_TARGET, VEC_UP);
    if (!cams.insert(std::pair<const char*, Camera*>(name, newCam)).second) {
        std::cout << "[WARNING] Camera already existing with name '" << name << "'" << std::endl;
    }
}


/* Func: SetActive
 * ---------------------------------------------------------
 * Sets camera with given name as the active camera
 *
 * name: ID of the camera to activate
 *
 * returns: true on success / false if no camera exists with given name
 */
bool CameraMan::SetActive(const char *name) {
    auto requestedElement = this->cams.find(name);
    if (requestedElement == cams.end()) {
        std::cout << "[WARNING] Trying to activate unexisting camera with name '" << name << "'" << std::endl;
        return false;
    }
    else {
        this->active_cam = requestedElement->second;
        this->active_name = requestedElement->first;
        return true;
    }
}


/* Func: Erase
 * ----------------------------------------------------------
 * Erases camera that identifies with given name, warns if it's inexistent
 *
 * name: ID of the camera to delete
 */
void CameraMan::Erase(const char *name) {
    
    auto toErase = this->cams.find(name);
    if (toErase == cams.end())
        std::cout << "[WARNING] Tried to delete inexistent camera '" << name << "'" << std::endl;
    else {
        auto next = this->cams.erase(toErase);
        
        /* if active camera has been deleted, next available one is activated */
        if (name == this->active_name) {
            this->active_cam = next->second;
            this->active_name = next->first;
        }
    }
}




/* Func: MoveCamera
 * -----------------------------------------------
 * Calls appropriate movement function for the active camera
 *
 * dir: direction of the movement
 * dist: distance to travel
 */
void CameraMan::MoveCamera(MovEnum dir, float dist) {
    switch (dir) {
        case MOVE_UP:
            active_cam->Up(dist);
            break;
            
        case MOVE_RIGHT:
            active_cam->Right(dist);
            break;
            
        case MOVE_FORWARD:
            active_cam->Forward(dist);
            break;
    }
}



/* Func: RotateCamera
 * --------------------------------------------------
 * Calls appropriate rotation function for the active camera
 *
 * dir: direction of rotation
 * angle: angle of rotation
 */
void CameraMan::RotateCamera(RotEnum dir, double angle) {
    switch (dir) {
        case ROT_PITCH:
            active_cam->Pitch(angle);
            break;
            
        case ROT_YAW:
            active_cam->Yaw(angle);
            break;
    }
}


/* Func: SendUniformMatrix
 * -------------------------------------------------
 * Sends active camera's matrix to stored location
 */
void CameraMan::SendUniformMatrix() {
    glm::mat4 matrix = active_cam->getMatrix();
    glUniformMatrix4fv(this->UniformCamLoc, 1, GL_FALSE, glm::value_ptr(matrix));
}


