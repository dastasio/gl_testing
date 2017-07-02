#pragma once
#include <vector>
#include "oglin.hpp"

struct Mesh {
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    GLuint numVertices;
    
    
    Mesh(std::vector<GLfloat> verts, std::vector<GLuint> inds) :
    vertices(verts), indices(inds) {
        this->numVertices = verts.size() / 8;
    }
    ~Mesh();
    
    
};
