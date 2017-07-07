#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <assimp/matrix4x4.h>
#include <string>
#include "oglin.hpp"

struct Mesh {
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    std::vector<std::string> tx_diffuse;
    std::vector<std::string> tx_specular;
    GLuint num_vertices;
    GLuint num_indices;
    size_t indices_offset;
    aiMatrix4x4 transform_mat;
    
    Mesh(std::vector<GLfloat> verts, std::vector<GLuint> inds) :
    vertices(verts), indices(inds) {
        this->num_vertices = verts.size() / 8;
        this->num_indices = inds.size();
        this->indices_offset = 0;
    }
    ~Mesh();
};
