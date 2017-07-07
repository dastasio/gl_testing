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
    GLuint vbo;
    
    void ProcessNode(aiNode *node, const aiScene* scene);
    void ProcessMesh(aiMesh *mesh, const aiScene* scene, aiMatrix4x4 transform);
    
    void SetAttribPointers(size_t offset);
    void InitBuffers();
};
