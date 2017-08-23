#include "light_manager.hpp"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

const int light_vao_index = 5;

LightMan::LightMan() {
    ProgramMan &pman = ProgramMan::instance();
    
    /* getting uniform location of variables from main program */
    this->UniformArrayLocation_point = pman.GetActiveUniformLocation("p_lights");
    this->UniformArrayLocation_dir = pman.GetActiveUniformLocation("dir_lights");
    this->UniformArraySizeLocation_point = pman.GetActiveUniformLocation("N_PLIGHTS");
    this->UniformArraySizeLocation_dir = pman.GetActiveUniformLocation("N_DLIGHTS");
    
    /* creating program for light rendering */
    pman.NewProgram("lights", "assets/light.vert", "assets/light.frag");
    this->UniformColorLocation = pman.GetActiveUniformLocation("lightColor");
    
    static bool vao_is_init = false;
    if (!vao_is_init) {
        LightMan::InitVAO();
        vao_is_init = true;
    }
}


void LightMan::NewPointLight(glm::vec3 p, glm::vec3 c) {
    this->point_lights.emplace_back(p, c);
}

void LightMan::NewDirLight(glm::vec3 dir, glm::vec3 c) {
    /* max 2 dir lights for project */
    if (this->dir_lights.size() >= 1)
        std::cout << "[INFO] Reached maximum number of directional lights!" << std::endl;
    else
        this->dir_lights.emplace_back(dir, c);
}

void LightMan::NewDirShadowMap(glm::vec3 dir, glm::vec3 c) {
    this->directional_shadow = new Light(dir, c);
    
    /* generating texture */
    glGenTextures(1, &shadow_map2D);
    glBindTexture(GL_TEXTURE_2D, shadow_map2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_MAP_WH, SHADOW_MAP_WH, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat border[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
    
    /* generating framebuffer */
    glGenFramebuffers(1, &shadow2D_fb);
    glBindFramebuffer(GL_FRAMEBUFFER, shadow2D_fb);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_map2D, 0);
    /* deactivating color buffer */
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightMan::NewPointShadowMap(glm::vec3 p, glm::vec3 c) {
    this->point_shadow = new Light(p, c);
    
    glGenFramebuffers(1, &shadowcube_fb);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowcube_fb);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    
    glGenTextures(1, &shadow_cubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadow_cubemap);
    for (int i = 0; i < 6; i++)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_MAP_WH, SHADOW_MAP_WH, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadow_cubemap, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    using glm::lookAt;
    glm::vec3 light_pos = point_shadow->position;
    glm::mat4 proj = glm::perspective(glm::radians(90.f), 1.f, 1.f, 25.f);
    lightSpaces.push_back(proj * lookAt(light_pos, light_pos + glm::vec3( 1.0, 0.0, 0.0), glm::vec3( 0.0,-1.0, 0.0)));
    lightSpaces.push_back(proj * lookAt(light_pos, light_pos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3( 0.0,-1.0, 0.0)));
    lightSpaces.push_back(proj * lookAt(light_pos, light_pos + glm::vec3( 0.0, 1.0, 0.0), glm::vec3( 0.0, 0.0, 1.0)));
    lightSpaces.push_back(proj * lookAt(light_pos, light_pos + glm::vec3( 0.0,-1.0, 0.0), glm::vec3( 0.0, 0.0,-1.0)));
    lightSpaces.push_back(proj * lookAt(light_pos, light_pos + glm::vec3( 0.0, 0.0, 1.0), glm::vec3( 0.0,-1.0, 0.0)));
    lightSpaces.push_back(proj * lookAt(light_pos, light_pos + glm::vec3( 0.0, 0.0,-1.0), glm::vec3( 0.0,-1.0, 0.0)));
}


void LightMan::MoveLight(glm::vec3 dir) {
    point_shadow->position += dir;
    lightSpaces.clear();
    using glm::lookAt;
    glm::vec3 light_pos = point_shadow->position;
    glm::mat4 proj = glm::perspective(glm::radians(90.f), 1.f, 1.f, 25.f);
    lightSpaces.push_back(proj * lookAt(light_pos, light_pos + glm::vec3( 1.0, 0.0, 0.0), glm::vec3( 0.0,-1.0, 0.0)));
    lightSpaces.push_back(proj * lookAt(light_pos, light_pos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3( 0.0,-1.0, 0.0)));
    lightSpaces.push_back(proj * lookAt(light_pos, light_pos + glm::vec3( 0.0, 1.0, 0.0), glm::vec3( 0.0, 0.0, 1.0)));
    lightSpaces.push_back(proj * lookAt(light_pos, light_pos + glm::vec3( 0.0,-1.0, 0.0), glm::vec3( 0.0, 0.0,-1.0)));
    lightSpaces.push_back(proj * lookAt(light_pos, light_pos + glm::vec3( 0.0, 0.0, 1.0), glm::vec3( 0.0,-1.0, 0.0)));
    lightSpaces.push_back(proj * lookAt(light_pos, light_pos + glm::vec3( 0.0, 0.0,-1.0), glm::vec3( 0.0,-1.0, 0.0)));
}
GLuint LightMan::MapPointShadows(void (*drawScene)()) {
    ProgramMan &pman = ProgramMan::instance();
    
    /* getting current viewport settings */
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    /* generating shadow map */
    glViewport(0, 0, SHADOW_MAP_WH, SHADOW_MAP_WH);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowcube_fb);
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);
    
    glUniformMatrix4fv(pman.GetActiveUniformLocation("shadowMatrices"), 6, GL_FALSE, glm::value_ptr(lightSpaces[0]));
    drawScene();
    
    /* restoring previous state */
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
    glCullFace(GL_BACK);
    return shadow_cubemap;
}

GLuint LightMan::MapDirShadows(void (*drawScene)(glm::mat4), glm::mat4 &lspaceMat) {
    /* getting current viewport settings */
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    /* generating shadow map */
    glViewport(0, 0, SHADOW_MAP_WH, SHADOW_MAP_WH);
    glBindFramebuffer(GL_FRAMEBUFFER, shadow2D_fb);
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);
    
    glm::mat4 projection = glm::ortho(-10.0, 10.0, -10.0, 10.0, 1.0, 7.5);
    glm::mat4 lightspace = glm::lookAt(-3.f * directional_shadow->position, glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));
    lspaceMat = projection * lightspace;
    drawScene(lspaceMat);
    
    /* restoring previous state */
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
    glCullFace(GL_BACK);
    return shadow_map2D;
}

void LightMan::CalculateLighting() {
    glUniform3fv(UniformArrayLocation_point, point_lights.size() * 4, glm::value_ptr(point_lights[0].position));
    glUniform1i(UniformArraySizeLocation_point, this->point_lights.size());
    
    ProgramMan &pman = ProgramMan::instance();
    glUniform3fv(pman.GetActiveUniformLocation("shadowLight_p"), 1, glm::value_ptr(point_shadow->position));
    glUniform3fv(pman.GetActiveUniformLocation("shadowLight_camb"), 1, glm::value_ptr(point_shadow->c_amb));
    glUniform3fv(pman.GetActiveUniformLocation("shadowLight_cdiff"), 1, glm::value_ptr(point_shadow->c_diff));
    glUniform3fv(pman.GetActiveUniformLocation("shadowLight_cspec"), 1, glm::value_ptr(point_shadow->c_spec));
    
    //glUniform3fv(UniformArrayLocation_dir, sizeof(directional_shadow), glm::value_ptr(directional_shadow->position));
//    glUniform1i(UniformArraySizeLocation_dir, 1);
}

void LightMan::RenderLights() {
    ProgramMan &pman = ProgramMan::instance();
    VAOMan &vman = VAOMan::instance();
    GLint prevVAO;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &prevVAO);
    vman.BindVAO(light_vao_index);
    
    GLint UniformModelMatrixLocation = pman.GetActiveUniformLocation("model");
    for (int i = 0; i < this->point_lights.size(); ++i) {
        glm::vec3 color = point_lights[i].c_diff;
        glm::mat4 transform = glm::mat4(1.0);
        transform = glm::translate(transform, point_lights[i].position);
        transform = glm::scale(transform, glm::vec3(0.2));
        
        glUniformMatrix4fv(UniformModelMatrixLocation, 1, GL_FALSE, glm::value_ptr(transform));
        glUniform3f(UniformColorLocation, color.r, color.g, color.b);
        glDrawElements(GL_TRIANGLES, 144, GL_UNSIGNED_INT, 0);
    }
    {
        glm::vec3 color = point_shadow->c_diff;
        glm::mat4 transform = glm::mat4(1.0);
        transform = glm::translate(transform, point_shadow->position);
        transform = glm::scale(transform, glm::vec3(0.2));
        
        glUniformMatrix4fv(UniformModelMatrixLocation, 1, GL_FALSE, glm::value_ptr(transform));
        glUniform3f(UniformColorLocation, color.r, color.g, color.b);
        glDrawElements(GL_TRIANGLES, 144, GL_UNSIGNED_INT, 0);
    }
    
    /* restoring previous state */
    glBindVertexArray(prevVAO);
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
}
