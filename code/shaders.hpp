#pragma once
#include "oglin.hpp"

struct Shader {
    Shader(const char* path) {
        this->ReadSource(path);
        this->path = path;
    }
    ~Shader() {
        delete this->src;
        glDeleteShader(this->compiled);
    }
    
    void ReadSource(const char* path);
    GLuint compile(GLenum type);
    
    const char* path;
    const char* src;
    int length;
    GLuint compiled;
};


class Program {
public:
    Program(const char* vert_path,
            const char* frag_path,
            const char* geom_path = nullptr);
    ~Program();
    
    void compile();
    void use();
    
    GLint getUniformLocation(const GLchar* name);
    GLuint getProgram() { return program; }
private:
    Shader vertex, fragment, geometry;
    GLuint program;
};
