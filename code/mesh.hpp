#pragma once
#include <vector>
#include "oglin.hpp"

struct Mesh {
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    std::vector<const GLchar*> tx_diffuse;
    std::vector<const GLchar*> tx_specular;
    GLuint num_vertices;
    GLuint num_indices;
    GLuint buf_offset;
    size_t indices_offset;
    
    Mesh(std::vector<GLfloat> verts, std::vector<GLuint> inds) :
    vertices(verts), indices(inds) {
        this->num_vertices = verts.size() / 8;
        this->num_indices = inds.size();
        this->buf_offset = 0;
        this->indices_offset = 0;
    }
    ~Mesh();
};
