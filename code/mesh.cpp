#include "mesh.hpp"

void Mesh::FreeDataForBuffers() {
    this->vertices.clear();
    this->indices.clear();
}
