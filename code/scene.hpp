#pragma once
#include "mesh.hpp"
#include <assimp/scene.h>

class Scene {
public:
    Scene(const char* path);
    ~Scene();
    
    void Draw();
private:
    std::vector<Mesh*> meshes;
    const char* directory;
    GLuint num_indices;
    
    void ProcessNode(aiNode *node, const aiScene* scene);
    void ProcessMesh(aiMesh *mesh, const aiScene* scene);
    
    void InitBuffers();
};