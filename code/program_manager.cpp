#include "program_manager.hpp"
#include <iostream>

void ProgramMan::NewProgram(const char *name, const char *vsh, const char *fsh, const char *gsh) {
    auto success = this->programs.insert(std::pair<const char*, Program*>(name, nullptr));
    if (!success.second) {
        std::cout << "[ERROR] Program already existing with name '" << name << "'" << std::endl;
        exit(EXIT_FAILURE);
    }
    else {
        /* if it's not a duplicate name, create new program */
        this->programs[name] = new Program(vsh, fsh, gsh);
        this->programs[name]->compile();
        this->active_name = name;
    }
}


void ProgramMan::Erase(const char *name) {
    auto deleturum = this->programs.find(name);
    
    if (deleturum == programs.end()) {
        std::cout << "[ERROR] Trying to delete inexistent program '" << name << "'" << std::endl;
    }
    else {
        this->programs.erase(deleturum);
    }
}

bool ProgramMan::SetActive(const char *name) {
    auto p = this->programs.find(name);
    
    if (p == programs.end()) {
        std::cout << "[ERROR] Cannot activate inexistent program '" << name << "'" << std::endl;
        return false;
    }
    
    p->second->use();
    this->active_name = name;
    return true;
}
