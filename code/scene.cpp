#include "scene.hpp"
#include "texture.hpp"
#include "program_manager.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <iostream>
#define BUF_OFFSET(x) (GLvoid*)(x)

TextureMan* tex_man = new TextureMan();

Scene::Scene(const char* path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs);
    
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "[ERROR] Assimp: " << importer.GetErrorString() << std::endl;
        exit(EXIT_FAILURE);
    }
    
    std::string dir(path);
    this->directory = dir.substr(0, dir.find_last_of('/')).c_str();
    
    ProcessNode(scene->mRootNode, scene);
    InitBuffers();
}

void Scene::ProcessNode(aiNode *node, const aiScene *scene) {
    for (GLuint i = 0; i < node->mNumMeshes; ++i) {
        aiMatrix4x4 transform_matrix = aiMatrix4x4(node->mTransformation);
        aiNode* parent = node->mParent;
        while (parent != nullptr) {
            transform_matrix *= parent->mTransformation;
            parent = parent->mParent;
        }
        /* getting mesh */
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        /* processing mesh */
        ProcessMesh(mesh, scene, transform_matrix);
    }
    
    /* processing subnodes */
    for (GLuint i = 0; i < node->mNumChildren; ++i) {
        ProcessNode(node->mChildren[i], scene);
    }
}


void Scene::ProcessMesh(aiMesh *mesh, const aiScene *scene, aiMatrix4x4 transform) {
    /* vectors containing data and indices of all the vertices */
    std::vector<GLfloat> v_data;
    std::vector<GLuint> v_indices;
    /* processing vertices */
    for (GLuint i = 0; i < mesh->mNumVertices; ++i) {
        /* vertex position */
        {
            aiVector3D v = mesh->mVertices[i];
            v_data.push_back(v.x);
            v_data.push_back(v.y);
            v_data.push_back(v.z);
        }
        
        /* vertex normal */
        {
            aiVector3D n = mesh->mNormals[i];
            v_data.push_back(n.x);
            v_data.push_back(n.y);
            v_data.push_back(n.z);
        }
        
        /* texture coordinates */
        {
            if (mesh->mTextureCoords[0]) {
                v_data.push_back(mesh->mTextureCoords[0][i].x);
                v_data.push_back(mesh->mTextureCoords[0][i].y);
            }
            else {
                v_data.push_back(0.0);
                v_data.push_back(0.0);
            }
        }
    }
    
    /* processing indices */
    for (GLuint i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (GLuint j = 0; j < face.mNumIndices; ++j) {
            v_indices.push_back(face.mIndices[j]);
        }
    }
    
    
    /* processing textures */
    aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<std::string> diffList;
    std::vector<std::string> specList;
    for (int i = 0; i < mat->GetTextureCount(aiTextureType_DIFFUSE); ++i) {
        aiString filename;
        mat->GetTexture(aiTextureType_DIFFUSE, i, &filename);
        std::string t_path(directory);
        t_path += filename.data[0] == '/' ? "" : "/";
        t_path += filename.C_Str();
        tex_man->AddTexture(t_path.c_str(), t_path);
        diffList.push_back(t_path);
    }
    for (int i = 0; i < mat->GetTextureCount(aiTextureType_SPECULAR); ++i) {
        aiString filename;
        mat->GetTexture(aiTextureType_SPECULAR, i, &filename);
        std::string t_path(directory);
        t_path += filename.data[0] == '/' ? "" : "/";
        t_path += filename.C_Str();
        tex_man->AddTexture(t_path.c_str(), t_path);
        specList.push_back(t_path);
    }
    
    this->meshes.push_back(new Mesh(v_data, v_indices));
    auto addedMesh = this->meshes[meshes.size() - 1];
    addedMesh->tx_diffuse = diffList;
    addedMesh->tx_specular = specList;
    addedMesh->transform_mat = transform;
    GLfloat *shininessPointer = &addedMesh->matShineFactor;
    *shininessPointer = 32;
}


void Scene::InitBuffers() {
    std::vector<GLfloat> total_vertices;
    std::vector<GLuint> total_indices;
    /* gathering data */
    GLuint num_vertices_so_far = 0;
    for (GLuint i = 0; i < meshes.size(); ++i) {
        total_vertices.insert(total_vertices.end(), meshes[i]->vertices.begin(), meshes[i]->vertices.end());
        meshes[i]->indices_offset = total_indices.size();
        for (auto &index : meshes[i]->indices) {
            total_indices.push_back(index + num_vertices_so_far);
        }
        num_vertices_so_far = total_vertices.size() / 8;
        meshes[i]->FreeDataForBuffers();
    }
    this->total_num_indices = total_indices.size();
    
    size_t fsize = sizeof(GLfloat);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, total_vertices.size() * fsize, total_vertices.data(), GL_STATIC_DRAW);
    
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, total_indices.size() * sizeof(GLuint), total_indices.data(), GL_STATIC_DRAW);
    
    this->SetPointers();
}

void Scene::SetPointers() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    
    size_t fsize = sizeof(GLfloat);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, fsize * 8, BUF_OFFSET(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, fsize * 8, BUF_OFFSET(3 * fsize));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, fsize * 8, BUF_OFFSET(6 * fsize));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Scene::Draw(GLboolean tex, glm::vec3 scale) {
    ProgramMan &prog_man = ProgramMan::instance();
    this->SetPointers();
    
    for (int i = 0; i < meshes.size(); ++i) {
        if (tex) {
            if (meshes[i]->tx_diffuse.size() > 0) {
                tex_man->Use(meshes[i]->tx_diffuse[0], 0, prog_man.GetActiveUniformLocation("mat.diffuse"));
            }
            if (meshes[i]->tx_specular.size() > 0) {
//                tex_man->Use(meshes[i]->tx_specular[0], 1, prog_man.GetActiveUniformLocation("mat.specular"));
            }
        }
        glm::mat4 model = TO_GLM_MATRIX(meshes[i]->transform_mat);
        model = glm::scale(glm::mat4(1.0), scale) * model;
        glUniformMatrix4fv(prog_man.GetActiveUniformLocation("model"), 1, GL_TRUE, &model[0][0]);
        
        GLuint offset = meshes[i]->indices_offset;
        GLsizei count = meshes[i]->num_indices;
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, BUF_OFFSET(offset * sizeof(GLuint)));
    }
}


void Scene::RenderSkybox(GLint location, GLint index) {
    tex_man->ActiveCubemap(location, index);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, BUF_OFFSET(0));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Scene::ActiveCubemap(GLint location, GLint index) {
    static bool hasBeenRead = false;
    if(!hasBeenRead) {
        tex_man->AddCubemap("assets/skybox");
        hasBeenRead = true;
    }
    
    tex_man->ActiveCubemap(location, index);
}
