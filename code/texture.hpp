#pragma once
#include "oglin.hpp"
#include <map>

struct Texture;

class TextureMan {
public:
    TextureMan();
    ~TextureMan();
    
    Texture ReadTexture(const char* path);
    void Add(const char* path, const char* name);
    
    void Use(const char* name, GLint index, GLint location);
private:
    std::map<const char*, GLuint> textures;
};
