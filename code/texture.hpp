#pragma once
#include "oglin.hpp"
#include <map>
#include <string>

struct Texture;

class TextureMan {
public:
    TextureMan();
    ~TextureMan();
    
    Texture ReadTexture(const char* path);
    void AddTexture(const char* path, std::string name);
    void AddCubemap(std::string pdir);
    
    void Use(std::string name, GLint index, GLint location);
    void ActiveCubemap(GLint location, GLint index);
    void ActiveShadowMap(GLint location, GLuint map);
    
private:
    std::map<std::string, GLuint> textures;
    GLuint cubemap;
};
