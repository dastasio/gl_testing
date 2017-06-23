#pragma once
#include "camera.hpp"
#include "shaders.hpp"
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
    CameraMan(Program* p);
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
    
private:
    std::map<const char*, Camera*> cams;
    Camera* active_cam;
    const char* active_name;
    
    GLint UniformCamLoc;
    Program *prog;
};
