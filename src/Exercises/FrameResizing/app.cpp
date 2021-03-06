#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Application/utils.h"

void SimpleShapeApplication::init() {

    float light_intensity = 2.8;
    float light_color[3] = {0.4, 0.2, 0.6};

    std::vector<GLushort> indices = {
            0, 1, 2, 0, 2, 3, //base
            5, 4, 6,
            7, 8, 9,
            11, 10, 12,
            13, 14, 15
    };

    std::vector<GLfloat> vertices = {
            //base (0-3)
            0.0f, 0.0f, 0.0f, 0.3, 0.4, 0.4,
            1.0f, 0.0f, 0.0f, 0.3, 0.4, 0.4,
            1.0f, 0.0f, 1.0f, 0.3, 0.4, 0.4,
            0.0f, 0.0f, 1.0f, 0.3, 0.4, 0.4,
            //side1 (4-6)
            0.0f, 0.0f, 0.0f, 0.5, 0.5, 0.2,
            1.0f, 0.0f, 0.0f, 0.5, 0.5, 0.2,
            0.5f, 1.0f, 0.5f, 0.5, 0.5, 0.2,
            //side2 (7-9)
            0.0f, 0.0f, 0.0f, 0.6, 0.2, 0.3,
            0.0f, 0.0f, 1.0f, 0.6, 0.2, 0.3,
            0.5f, 1.0f, 0.5f, 0.6, 0.2, 0.3,
            //side3 (10-12)
            1.0f, 0.0f, 1.0f, 0.0, 0.5, 0.1,
            0.0f, 0.0f, 1.0f, 0.0, 0.5, 0.1,
            0.5f, 1.0f, 0.5f, 0.0, 0.5, 0.1,
            //side4 (13-15)
            1.0f, 0.0f, 1.0f, 0.3, 0.0, 0.1,
            1.0f, 0.0f, 0.0f, 0.3, 0.0, 0.1,
            0.5f, 1.0f, 0.5f, 0.3, 0.0, 0.1,
    };

    auto program = xe::create_program(std::string(PROJECT_DIR) + "/shaders/base_vs.glsl",
                                      std::string(PROJECT_DIR) + "/shaders/base_fs.glsl");

    if (!program) {
        std::cerr << "Cannot create program from " << std::string(PROJECT_DIR) + "/shaders/base_vs.glsl" << " and ";
        std::cerr << std::string(PROJECT_DIR) + "/shaders/base_fs.glsl" << " shader files" << std::endl;
    }

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    GLuint idx_buffer_handle;
    glGenBuffers(1, &idx_buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buffer_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    GLuint v_buffer_handle;
    glGenBuffers(1, &v_buffer_handle);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buffer_handle);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
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

    aspect_ = (float)w/h;
    fov_ = glm::pi<float>()/4.0;
    near_ = 0.1f;
    far_ = 100.0f;
    P_ = glm::perspective(fov_, aspect_, near_, far_);
    V_ = glm::lookAt(glm::vec3(4.0f, -0.5f, -0.8f),
                     glm::vec3(0.0f, 0.3f, 0.5f),
                     glm::vec3(0.0f, 0.5f, 0.0f));


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

    glGenBuffers(1, &pvm_buffer_);
    glBindBuffer(GL_UNIFORM_BUFFER, pvm_buffer_);
    glBufferData(GL_UNIFORM_BUFFER, 4 * 4 * sizeof(float), nullptr, GL_STATIC_DRAW);

    glViewport(0, 0, w, h);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);
}

void SimpleShapeApplication::frame() {
    auto PVM = P_ * V_;
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, pvm_buffer_);

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid *>(0));
    glBindVertexArray(0);
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0,0,w,h);
    aspect_ = (float) w / h;
    P_ = glm::perspective(fov_, aspect_, near_, far_);
}
