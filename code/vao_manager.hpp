#pragma once
#include "oglin.hpp"
#include <map>

class VAOMan {
public:
    static VAOMan &instance() {
        static VAOMan *instance = new VAOMan();
        return *instance;
    }
    
    void NewVAO(int ID);
    void EraseVAO(int ID);
    void BindVAO(int ID);
    void Unbind();
    
    void EnableAttrib(GLuint count, GLuint *indices);
    void DisableAttrib(GLuint count, GLuint *indices);
private:
    VAOMan() {}
    ~VAOMan() {}
    
    std::map<int, GLuint> VAOs;
};
