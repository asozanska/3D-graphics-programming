//
// Created by ada on 26.11.2020.
//
#pragma once

#include <vector>
#include "glad/glad.h"


class Pyramid  {
public:
    Pyramid();
    ~Pyramid() = default;
    Pyramid(const Pyramid& rhs) = delete;
    Pyramid &operator=(const Pyramid& rhs) = delete;

    void operator=(Pyramid&& rhs) = delete;
    Pyramid(Pyramid&& rhs) = delete;
    void draw(glm::mat4 &PVM, GLuint buffer_);
private:
    GLuint vao_;
    GLuint buffer_[2];
    GLuint diffuse_texture_;

    std::vector<GLushort> indices = {
            0, 1, 2, 0, 2, 3, //base
            1, 0, 6,
            0, 3, 7,
            3, 2, 4,
            2, 1, 5
    };

    std::vector<GLfloat> vertices = {
            //base (0-3)
            -0.5f, -0.5f, -0.5f, 0.191, 0.5,
            0.5f, -0.5f, -0.5f, 0.5, 0.809,
            0.5f, -0.5f, 0.5f, 0.809, 0.5,
            -0.5f, -0.5f, 0.5f, 0.5, 0.191,

            0.0f, 0.5f, 0.0f, 1.0, 0.0,
            0.0f, 0.5f, 0.0f, 1.0, 1.0,
            0.0f, 0.5f, 0.0f, 0.0, 1.0,
            0.0f, 0.5f, 0.0f, 0.0, 0.0
    };
};