//
// Created by pbialas on 25.09.2020.
//

#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "Application/utils.h"

void SimpleShapeApplication::init() {

    float light_intensity = 0.9;
    float light_color[3] = {0.3, 0.1, 0.4};

    std::vector<GLushort> indices = {
            0, 1, 2, 3, 4, 5, 5, 6, 3
    };

    std::vector<GLfloat> vertices = {
            0.6f, -0.5f, 0.0f, 0.2, 0.3, 0.7,
            0.2f, 0.1f, 0.0f, 0.2, 0.3, 0.7,
            -0.2f, -0.5f, 0.0f, 0.2, 0.3, 0.7,

            0.6f, -0.5f, 0.0f,  0.9, 0.7, 0.2,
            -0.2f, -0.5f, 0.0f,  0.9, 0.7, 0.2,

            -0.2f, -0.9f, 0.0f, 0.9, 0.7, 0.2,
            0.6f, -0.9f, 0.0f, 0.9, 0.7, 0.2,

    };

    auto program = xe::create_program(std::string(PROJECT_DIR) + "/shaders/base_vs.glsl",
                                      std::string(PROJECT_DIR) + "/shaders/base_fs.glsl");

    if (!program) {
        std::cerr << "Cannot create program from " << std::string(PROJECT_DIR) + "/shaders/base_vs.glsl" << " and ";
        std::cerr << std::string(PROJECT_DIR) + "/shaders/base_fs.glsl" << " shader files" << std::endl;
    }

    GLuint idx_buffer_handle;
    glGenBuffers(1,&idx_buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buffer_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    GLuint v_buffer_handle;
    glGenBuffers(1, &v_buffer_handle);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buffer_handle);
    glBindVertexArray(0);

    auto u_transformations_index = glGetUniformBlockIndex(program, "Transformations");
    if (u_transformations_index == GL_INVALID_INDEX) {
        std::cout << "Cannot find Transformations uniform block in program" << std::endl;
    } else {
        glUniformBlockBinding(program, u_transformations_index, 0);
    }
    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    int w, h;
    std::tie(w, h) = frame_buffer_size();

    glm::mat4 M(1.0f);
    glm::mat4 V = glm::lookAt(glm::vec3(0.9f, 1.2f, 1.4f),
                              glm::vec3(1.0f, 0.3f, 0.9f),
                              glm::vec3(0.7f, 1.3f, 0.0f)
    );
    glm::mat4 P = glm::perspective(glm::radians(65.f), (float) w / h, 1.0f, 150.f);
    glm::mat4 PVM = P * V * M;

    GLuint ubo_handle_t(0u);
    glGenBuffers(1,&ubo_handle_t);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_handle_t);
    glBufferData(GL_UNIFORM_BUFFER, 4 * 4 * sizeof(float), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, 4 * 4 *sizeof(float), &PVM[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo_handle_t);

    auto u_modifiers_index = glGetUniformBlockIndex(program, "Modifiers");
    if (u_modifiers_index == GL_INVALID_INDEX) {
        std::cout << "Cannot find Modifiers uniform block in program" << std::endl;
    } else {
        glUniformBlockBinding(program, u_modifiers_index, 1);
    }

    GLuint ubo_handle_m(0u);
    glGenBuffers(1, &ubo_handle_m);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_handle_m);
    glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(float), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &light_intensity);
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(float), 3 * sizeof(float), light_color);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, ubo_handle_m);

    glViewport(0, 0, w, h);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);
}

void SimpleShapeApplication::frame() {
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid *>(0));
    glBindVertexArray(0);
}
