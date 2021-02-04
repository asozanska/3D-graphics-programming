//
// Created by ada on 26.11.2020.
//

#include <string>
#include "quad.h"

Quad::Quad() {
    glGenBuffers(1, &buffer_[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenBuffers(1, &buffer_[1]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_[1]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_[1]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_[0]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(5 * sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Quad::draw() {
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid *>(0));
}

void Quad::setMaterial(glm::vec3 kd, uint32_t kd_map, glm::vec3 ks, uint32_t ks_map, float ns, uint32_t ns_map) {
    material_ = new PhongMaterial();
    material_->Kd = kd;
    material_->Kd_map = kd_map;
    material_->Ks = ks;
    material_->Ks_map = ks_map;
    material_->Ns= ns;
    material_->Ns_map = ns_map;
}
PhongMaterial* Quad::getMaterial() {
    return material_;
}