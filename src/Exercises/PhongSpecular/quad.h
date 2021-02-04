//
// Created by ada on 26.11.2020.
//
#pragma once

#include <vector>
#include "glad/glad.h"
#include "phong_material.h"


class Quad  {
public:
    Quad();
    ~Quad() = default;
    Quad(const Quad& rhs) = delete;
    Quad &operator=(const Quad& rhs) = delete;

    void operator=(Quad&& rhs) = delete;
    Quad(Quad&& rhs) = delete;
    void draw();
    void setMaterial(glm::vec3 kd, uint32_t kd_map, glm::vec3 ks, uint32_t ks_map, float ns, uint32_t ns_map);
    PhongMaterial* getMaterial();
private:
    GLuint vao_;
    GLuint buffer_[2];
    PhongMaterial *material_;

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