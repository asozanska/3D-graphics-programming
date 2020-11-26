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

    std::vector<GLushort> indices = {
            0, 1, 2, 0, 2, 3, //base
            5, 4, 6,
            7, 8, 9,
            11, 10, 12,
            13, 14, 15
    };

    std::vector<GLfloat> vertices = {
            //base (0-3)
            -0.5f, -0.5f, -0.5f, 0.3, 0.4, 0.4,
            0.5f, -0.5f, -0.5f, 0.3, 0.4, 0.4,
            0.5f, -0.5f, 0.5f, 0.3, 0.4, 0.4,
            -0.5f, -0.5f, 0.5f, 0.3, 0.4, 0.4,
            //side1 (4-6)
            -0.5f, -0.5f, -0.5f, 0.5, 0.5, 0.2,
            0.5f, -0.5f, -0.5f, 0.5, 0.5, 0.2,
            0.0f, 0.5f, 0.0f, 0.5, 0.5, 0.2,
            //side2 (7-9)
            -0.5f, -0.5f, -0.5f, 0.6, 0.2, 0.3,
            -0.5f, -0.5f, 0.5f, 0.6, 0.2, 0.3,
            0.0f, 0.5f, 0.0f, 0.6, 0.2, 0.3,
            //side3 (10-12)
            0.5f, -0.5f, 0.5f, 0.0, 0.5, 0.1,
            -0.5f, -0.5f, 0.5f, 0.0, 0.5, 0.1,
            0.0f, 0.5f, 0.0f, 0.0, 0.5, 0.1,
            //side4 (13-15)
            0.5f, -0.5f, 0.5f, 0.3, 0.0, 0.1,
            0.5f, -0.5f, -0.5f, 0.3, 0.0, 0.1,
            0.0f, 0.5f, 0.0f, 0.3, 0.0, 0.1,
    };
};