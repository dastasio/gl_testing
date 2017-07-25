#pragma once
#include "mesh.hpp"
#include <assimp/scene.h>

class Scene {
public:
    Scene(const char* path);
    ~Scene();
    
    void Draw(GLboolean tex, glm::vec3 scale);
    void RenderSkybox(GLint location, GLint index = 0);
    void ActiveCubemap(GLint location, GLint index = 0);
private:
    std::vector<Mesh*> meshes;
    const char* directory;
    GLuint vbo, ebo;
    GLuint total_num_indices;
    
    void ProcessNode(aiNode *node, const aiScene* scene);
    void ProcessMesh(aiMesh *mesh, const aiScene* scene, aiMatrix4x4 transform);
    
    void InitBuffers();
    void SetPointers();
};




#define TO_GLM_MATRIX(x) glm::mat4(x.a1, x.a2, x.a3, x.a4, x.b1, x.b2, x.b3, x.b4, x.c1, x.c2, x.c3, x.c4, x.d1, x.d2, x.d3, x.d4)
