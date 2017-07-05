#include "light_manager.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

const int light_vao_index = 5;

LightMan::LightMan() {
    ProgramMan &pman = ProgramMan::instance();
    
    /* getting uniform location of variables from main program */
    this->UniformArrayLocation = pman.GetActiveUniformLocation("lights");
    this->UniformArraySizeLocation = pman.GetActiveUniformLocation("N_LIGHTS");
    
    /* creating program for light rendering */
    pman.NewProgram("lights", "assets/light.vert", "assets/light.frag");
    this->UniformColorLocation = pman.GetActiveUniformLocation("lightColor");
    
    static bool vao_is_init = false;
    if (!vao_is_init) {
        LightMan::InitVAO();
        vao_is_init = true;
    }
}

int LightMan::NewLight(glm::vec3 p, glm::vec3 c) {
    this->lights.push_back(Light(p.x, p.y, p.z, c.r, c.g, c.b));
    return lights.size() - 1;
}

void LightMan::EraseLight(int light_id) {
    this->lights.erase(this->lights.begin() + light_id);
}


void LightMan::CalculateLighting() {
    glUniform3fv(UniformArrayLocation, lights.size() * 2, glm::value_ptr(lights[0].position));
    glUniform1i(UniformArraySizeLocation, this->lights.size());
}

void LightMan::RenderLights() {
    ProgramMan &pman = ProgramMan::instance();
    VAOMan &vman = VAOMan::instance();
    pman.SetActive("lights");
    vman.BindVAO(light_vao_index);
    
    GLint UniformModelMatrixLocation = pman.GetActiveUniformLocation("model");
    for (int i = 0; i < this->lights.size(); ++i) {
        glm::vec3 color = lights[i].color;
        glm::mat4 transform = glm::mat4(1.0);
        transform = glm::translate(transform, lights[i].position);
        transform = glm::scale(transform, glm::vec3(0.2));
        
        glUniformMatrix4fv(UniformModelMatrixLocation, 1, GL_FALSE, glm::value_ptr(transform));
        glUniform3f(UniformColorLocation, color.r, color.g, color.b);
        glDrawElements(GL_TRIANGLES, 144, GL_UNSIGNED_INT, 0);
    }
    vman.Unbind();
}


void LightMan::InitVAO() {
    VAOMan &vaoman = VAOMan::instance();
    GLfloat vertices[] = {
        0.0, 0.0, 0.0,
         0.000000, -1.000000,  0.000000,
         0.500000,  0.707107, -0.500000,
         0.707107, -0.000000, -0.707107,
         0.500000, -0.707107, -0.500000,
         0.707107,  0.707107,  0.000000,
         1.000000, -0.000000,  0.000000,
         0.707107, -0.707107,  0.000000,
         0.500000,  0.707107,  0.500000,
         0.707107, -0.000000,  0.707107,
         0.500000, -0.707107,  0.500000,
         0.000000,  0.707107,  0.707107,
         0.000000, -0.000000,  1.000000,
         0.000000, -0.707107,  0.707107,
        -0.000000,  1.000000,  0.000000,
        -0.500000,  0.707107,  0.500000,
        -0.707107, -0.000000,  0.707107,
        -0.500000, -0.707107,  0.500000,
        -0.707107,  0.707107,  0.000000,
        -1.000000, -0.000000,  0.000000,
        -0.707107, -0.707107,  0.000000,
        -0.500000,  0.707107, -0.500000,
        -0.707107, -0.000000, -0.707107,
        -0.500000, -0.707107, -0.500000,
         0.000000,  0.707107, -0.707107,
         0.000000, -0.000000, -1.000000,
         0.000000, -0.707107, -0.707107
    };
    GLuint indices[] = {
         1, 26,  4,     24,  3, 25,     25,  4, 26,
        24, 14,  2,      1,  4,  7,      2,  6,  3,
         3,  7,  4,      2, 14,  5,      1,  7, 10,
         5,  9,  6,      6, 10,  7,      5, 14,  8,
         1, 10, 13,      8, 12,  9,      9, 13, 10,
         8, 14, 11,      1, 13, 17,     11, 16, 12,
        12, 17, 13,     11, 14, 15,     15, 14, 18,
         1, 17, 20,     15, 19, 16,     16, 20, 17,
        19, 23, 20,     18, 14, 21,      1, 20, 23,
        19, 21, 22,     22, 26, 23,     21, 14, 24,
         1, 23, 26,     22, 24, 25,     24,  2,  3,
        25,  3,  4,      2,  5,  6,      3,  6,  7,
         5,  8,  9,      6,  9, 10,      8, 11, 12,
         9, 12, 13,     11, 15, 16,     12, 16, 17,
        15, 18, 19,     16, 19, 20,     19, 22, 23,
        19, 18, 21,     22, 25, 26,     22, 21, 24
    };
    
    vaoman.NewVAO(light_vao_index);
    GLuint vbo, ebo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    GLuint arr[1] = {0};
    vaoman.EnableAttrib(1, arr);
    vaoman.Unbind();
}

LightMan::~LightMan() {
    delete &this->UniformArraySizeLocation;
    delete &this->UniformArrayLocation;
    lights.clear();
    delete &this->lights;
}
