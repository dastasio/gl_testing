#include "texture.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>


struct Texture {
    GLint w, h;
    GLubyte* pixels;
    GLenum format;
};

TextureMan::TextureMan() {
    
}

TextureMan::~TextureMan() {
    
}


Texture TextureMan::ReadTexture(const char *path) {
    SDL_Surface* image = IMG_Load(path);
    if (image == nullptr) {
        std::cerr << "[ERROR] Texture '" << path << "' not found:\n" << IMG_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    
    
    Texture tx;
    tx.w = image->w;
    tx.h = image->h;
    tx.pixels = (GLubyte*)image->pixels;
    tx.format = SDL_PIXELORDER(image->format->format) == SDL_ARRAYORDER_RGB ? GL_RGB : GL_RGBA;
    SDL_FreeSurface(image);
    return tx;
}


void TextureMan::Add(const char *path, const char *name) {
    Texture tx = ReadTexture(path);
    
    /* texture generation */
    GLuint glTexture;
    glGenTextures(1, &glTexture);
    glBindTexture(GL_TEXTURE_2D, glTexture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tx.w, tx.h, 0, tx.format, GL_UNSIGNED_BYTE, tx.pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    
    /* adding texture to map */
    if (!this->textures.insert(std::pair<const char*, GLuint>(name, glTexture)).second) {
        std::cout << "[WARNING] A texture named '" << name << "' already exists!" << std::endl;
    }
}


void TextureMan::Use(const char *name, GLint index, GLint location) {
    auto txIterator = this->textures.find(name);
    if (txIterator == textures.end()) {
        std::cout << "[WARNING] Tried to activate inexisting texture '" << name << "'" << std::endl;
    }
    else {
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_2D, txIterator->second);
        
        glUniform1i(location, index);
    }
}
