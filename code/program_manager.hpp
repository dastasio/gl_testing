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
    void NewProgram(const char* name, const char* vsh, const char* fsh, const char *gsh = nullptr);
    bool SetActive(const char* name);
    void Erase(const char* name);
    
    /* this is temporary */
    Program* getProgram(const char* name) {
        return programs[name];
    }
    GLint GetActiveUniformLocation(const char* req) {
        return programs[active_name]->getUniformLocation(req);
    }
    
    /* functionalities */
    
private:
    ProgramMan() {}
    ~ProgramMan() {}
    
    std::map<const char*, Program*> programs;
    const char* active_name;
};
