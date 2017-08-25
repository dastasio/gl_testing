#pragma once
#include "camera.hpp"
#include "program_manager.hpp"
#include <map>

enum MovEnum {
    MOVE_UP,
    MOVE_RIGHT,
    MOVE_FORWARD
};

enum RotEnum {
    ROT_PITCH,
    ROT_YAW
};

class CameraMan {
public:
    CameraMan();
    ~CameraMan();
    
    /* modifiers */
    void Add(const char* name, glm::vec3 pos);
    bool SetActive(const char* name);
    void Erase(const char* name);
    
    /* camera control */
    void MoveCamera(MovEnum dir, float dist);
    void RotateCamera(RotEnum dir, double angle);
    
    /* functionalities */
    void SendUniformMatrix();
    void SendEyePosition();
    
private:
    std::map<const char*, Camera*, cmp_str> cams;
    Camera* active_cam;
    const char* active_name;
};
