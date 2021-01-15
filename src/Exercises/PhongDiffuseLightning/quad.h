//
// Created by ada on 26.11.2020.
//
#pragma once

#include <vector>
#include "glad/glad.h"


class Quad  {
public:
    Quad();
    ~Quad() = default;
    Quad(const Quad& rhs) = delete;
    Quad &operator=(const Quad& rhs) = delete;

    void operator=(Quad&& rhs) = delete;
    Quad(Quad&& rhs) = delete;
    void draw();
private:
    GLuint vao_;
    GLuint buffer_[2];
    GLuint diffuse_texture_;

    std::vector<GLushort> indices = {
            0, 1, 2,
            0, 2, 3,
    };

    std::vector<GLfloat> vertices = {
            -1.0f, 0.0f, -1.0f, 0.0, 0.0, 0.0, 1.0, 0.0,
            -1.0f, 0.0f, 1.0f, 0.0, 1.0, 0.0, 1.0, 0.0,
            1.0f, 0.0f, 1.0f, 1.0, 1.0, 0.0, 1.0, 0.0,
            1.0f, 0.0f, -1.0f, 1.0, 0.0, 0.0, 1.0, 0.0
    };
};