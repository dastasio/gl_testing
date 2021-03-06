#include "shaders.hpp"
#include <fstream>
#include <iostream>

/* Func: ReadSource()
 * --------------------------------------------
 * Reads text file into char* variable
 *
 * path: path of the file to read
 */
void Shader::ReadSource(const char *path) {
    std::ifstream file(path, std::ios::in);
    
    if (!file.good()) {
        std::cerr << "[ERROR] Could not open " << path << std::endl;
        exit(EXIT_FAILURE);
    }
    
    file.seekg(0, std::ios::end);
    uint l = file.tellg();
    char* source = new char[l];
    
    file.seekg(0, std::ios::beg);
    file.read(source, l);
    
    this->src = source;
    this->length = l;
}


/* Constructor: Program
 * ----------------------------------------------
 * creates empty program and reads shaders' sources
 */
Program::Program(const char* vert_path, const char* frag_path) :
vertex(vert_path), fragment(frag_path) {
}



void Program::compile() {
    GLuint vert = this->vertex.compile(GL_VERTEX_SHADER);
    GLuint frag = this->fragment.compile(GL_FRAGMENT_SHADER);
    
    program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);
    
    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        int log_size = 0;
        glGetShaderiv(program, GL_INFO_LOG_LENGTH, &log_size);
        char* log = new char[log_size];
        glGetProgramInfoLog(program, log_size, nullptr, log);
        std::cerr << "[ERROR] Could not link program:\n" << log << std::endl;
        exit(EXIT_FAILURE);
    }
    
    this->vertex.~Shader();
    this->fragment.~Shader();
}


/* Func: Use
 * ---------------------------------------
 * sets its program as the currently active one
 */
void Program::use() {
    glUseProgram(this->program);
}


GLuint Shader::compile(GLenum type) {
    /* creating shader */
    GLuint sh = glCreateShader(type);
    glShaderSource(sh, 1, &this->src, &this->length);
    glCompileShader(sh);
    
    /* checking compilation success */
    GLint success = 0;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        int log_size = 0;
        glGetShaderiv(sh, GL_INFO_LOG_LENGTH, &log_size);
        char* log = new char[log_size];
        glGetShaderInfoLog(sh, log_size, nullptr, log);
        std::cerr << "[ERROR] Could not compile " << this->path << ":\n" << log << std::endl;
        exit(EXIT_FAILURE);
    }
    
    return sh;
}


/* Func: getUniformLocation
 * ---------------------------------------------------------
 * gets location of the uniform variable with given name
 *
 * name: name of the variable whose location is requested
 *
 * returns: location of the variable on success / -1 on failure
 */
GLint Program::getUniformLocation(const GLchar *name) {
    GLint loc = glGetUniformLocation(this->program, name);
    if (loc == -1)
        std::cout << "[ERROR] Requested location of inexisting variable '" << name << "'" << std::endl;
    return loc;
}










