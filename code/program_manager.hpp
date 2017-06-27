#pragma once
#include "shaders.hpp"
#include <map>

class ProgramMan {
public:
    static ProgramMan& instance() {
        static ProgramMan* inst = new ProgramMan();
        return *inst;
    }
    
    /* modifiers */
    void NewProgram(const char* name, const char* vsh, const char* fsh);
    bool SetActive(const char* name);
    void Erase(const char* name);
    
    
    /* functionalities */
    
private:
    ProgramMan() {}
    ~ProgramMan() {}
    
    std::map<const char*, Program*> programs;
    const char* active_name;
    
    GLint UniformCamLoc;
};
