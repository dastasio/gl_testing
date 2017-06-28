#pragma once
#include "oglin.hpp"
#include <map>

class VAOMan {
public:
    VAOMan() {}
    ~VAOMan() {}
    
    void NewVAO(int ID);
    void EraseVAO(int ID);
    void BindVAO(int ID);
    void Unbind();
    
private:
    std::map<int, GLuint> VAOs;
};
