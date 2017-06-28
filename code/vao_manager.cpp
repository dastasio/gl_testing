#include "vao_manager.hpp"
#include <iostream>

void VAOMan::NewVAO(int ID) {
    bool duplicate = !this->VAOs.insert(std::pair<int,GLuint>(ID, 0)).second;
    
    if (duplicate) {
        std::cout << "[ERROR] A VAO already exists with ID '" << ID << "'" << std::endl;
    }
    else {
        glGenVertexArrays(1, &VAOs[ID]);
        BindVAO(ID);
    }
}


void VAOMan::EraseVAO(int ID) {
    auto to_erase = VAOs.find(ID);
    
    if (to_erase == VAOs.end()) {
        std::cout << "[ERROR] Tried to erase inexisting VAO '" << ID << "'" << std::endl;
    }
    else {
        VAOs.erase(to_erase);
    }
}


void VAOMan::BindVAO(int ID) {
    auto to_bind = VAOs.find(ID);
    
    if (to_bind == VAOs.end()) {
        std::cout << "[ERROR] Tried to bind inexisting VAO '" << ID << "'" << std::endl;
    }
    else {
        glBindVertexArray(to_bind->second);
    }
}

void VAOMan::Unbind() {
    glBindVertexArray(0);
}
