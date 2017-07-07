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
    void Add(const char* path, std::string name);
    
    void Use(std::string name, GLint index, GLint location);
private:
    std::map<std::string, GLuint> textures;
};
