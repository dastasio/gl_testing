#include "scene.hpp"
#include "texture.hpp"
#include "program_manager.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <string>
#define BUF_OFFSET(x) (GLvoid*)(x)

TextureMan* tex_man = new TextureMan();

Scene::Scene(const char* path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices);
    
    std::string dir(path);
    this->directory = dir.substr(0, dir.find_last_of('/')).c_str();
    
    ProcessNode(scene->mRootNode, scene);
    InitBuffers();
}

void Scene::ProcessNode(aiNode *node, const aiScene *scene) {
    for (GLuint i = 0; i < node->mNumMeshes; ++i) {
        /* getting mesh */
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        /* processing mesh */
        ProcessMesh(mesh, scene);
    }
    
    /* processing subnodes */
    for (GLuint i = 0; i < node->mNumChildren; ++i) {
        ProcessNode(node->mChildren[i], scene);
    }
}


void Scene::ProcessMesh(aiMesh *mesh, const aiScene *scene) {
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
            GLfloat tc_x = (mesh->mTextureCoords[0]) ? mesh->mTextureCoords[0][i].x : 0.0;
            GLfloat tc_y = (mesh->mTextureCoords[0]) ? mesh->mTextureCoords[0][i].y : 0.0;
            v_data.push_back(tc_x);
            v_data.push_back(tc_y);
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
    std::vector<const GLchar*> textureList;
    for (int i = 0; i < mat->GetTextureCount(aiTextureType_DIFFUSE); ++i) {
        aiString t_path;
        mat->GetTexture(aiTextureType_DIFFUSE, i, &t_path);
        std::string texture_path(directory);
        texture_path += t_path.C_Str();
        tex_man->Add(texture_path.c_str(), texture_path.c_str());
        textureList.push_back(texture_path.c_str());
    }
    
    this->meshes.push_back(new Mesh(v_data, v_indices));
    this->meshes[meshes.size() - 1]->textures = textureList;
}


void Scene::InitBuffers() {
    std::vector<GLfloat> total_vertices;
    std::vector<GLuint> total_indices;
    /* gathering data */
    for (GLuint i = 0; i < meshes.size(); ++i) {
        meshes[i]->buf_offset = total_vertices.size();
        meshes[i]->indices_offset = total_indices.size();
        total_vertices.insert(total_vertices.end(), meshes[i]->vertices.begin(), meshes[i]->vertices.end());
        total_indices.insert(total_indices.end(), meshes[i]->indices.begin(), meshes[i]->indices.end());
    }
    
    size_t fsize = sizeof(GLfloat);
    GLuint ebo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, total_vertices.size() * fsize, total_vertices.data(), GL_STATIC_DRAW);
    
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, total_indices.size() * sizeof(GLuint), total_indices.data(), GL_STATIC_DRAW);
}

void Scene::SetAttribPointers(size_t offset) {
    size_t fsize = sizeof(GLfloat);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, fsize * 8, BUF_OFFSET(offset));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, fsize * 8, BUF_OFFSET(offset + 3 * fsize));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, fsize * 8, BUF_OFFSET(offset + 6 * fsize));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
}

void Scene::Draw() {
    ProgramMan &prog_man = ProgramMan::instance();
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    for (int i = 0; i < meshes.size(); ++i) {
        if (meshes[i]->textures.size() > 0) {
            tex_man->Use(meshes[i]->textures[0], 0, prog_man.GetActiveUniformLocation("mat.diffuse"));
        }
        
        SetAttribPointers(meshes[i]->buf_offset);
        GLuint start = meshes[i]->indices_offset;
        GLsizei count = meshes[i]->num_indices;
        GLuint end = start + count;
        glDrawRangeElements(GL_TRIANGLES, start, end, count, GL_UNSIGNED_INT, 0);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

