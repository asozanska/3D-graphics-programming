#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Application/utils.h"

void SimpleShapeApplication::init() {
    auto program = xe::create_program(std::string(PROJECT_DIR) + "/shaders/base_vs.glsl",
                                      std::string(PROJECT_DIR) + "/shaders/base_fs.glsl");

    if (!program) {
        std::cerr << "Cannot create program from " << std::string(PROJECT_DIR) + "/shaders/base_vs.glsl" << " and ";
        std::cerr << std::string(PROJECT_DIR) + "/shaders/base_fs.glsl" << " shader files" << std::endl;
    }

    set_camera(new Camera);
    set_controler(new CameraControler(camera()));

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    quad = new Quad;
    quad->setMaterial(glm::vec3(1.0, 0.0, 0.0), 0, glm::vec3(0.05, 0.05, 0.05), 0 , 1000.0f, 0);

    xe::utils::set_uniform_block_binding(program, "Transformations", 0);
    xe::utils::set_uniform_block_binding(program, "Material", 1);
    xe::utils::set_uniform_block_binding(program, "Light", 2);

    light.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    light.a = glm::vec4(1.0f, 0.0f, 1.0f, 0.0f);
    light.ambient = glm::vec3(0.2f, 0.2f, 0.2f);

    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    int w, h;
    std::tie(w, h) = frame_buffer_size();

    camera_->perspective(glm::pi<float>()/4.0, (float)w/h, 0.1f, 100.0f);
    camera_->look_at(glm::vec3(4.0f, 4.0f, -0.8f),
                     glm::vec3(0.0f, 0.3f, 0.5f),
                     glm::vec3(0.0f, 0.5f, 0.0f));


    glGenBuffers(1, &light_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, light_buffer);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(Light), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, light_buffer);

    glGenBuffers(1, &material_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, material_buffer);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(PhongMaterial), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, material_buffer);

    glGenBuffers(1, &pvm_buffer_);
    glBindBuffer(GL_UNIFORM_BUFFER, pvm_buffer_);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + 3 * sizeof(glm::vec4), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, pvm_buffer_);

    GLuint diffuse_texture;
    glGenTextures(1, &diffuse_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse_texture);
    xe::utils::load_texture(std::string(PROJECT_DIR) + "/plastic.png");
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint shininess_texture;
    glGenTextures(1, &shininess_texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shininess_texture);
    xe::utils::load_texture(std::string(PROJECT_DIR) + "/shininess.png");
    glBindTexture(GL_TEXTURE_2D, 0);

    glViewport(0, 0, w, h);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);

    xe::utils::set_uniform1i(program, "diffuse_map", 0);
    xe::utils::set_uniform1i(program,"specular_map",1);
    xe::utils::set_uniform1i(program,"shininess_map",2);
}

void SimpleShapeApplication::frame() {
    auto P = camera_->projection();
    auto VM = camera_->view();
    auto R = glm::mat3(VM);
    auto N = glm::transpose(glm::inverse(R));
    light.position = glm::vec4 (0.0f, 1.0f, 0.0f, 1.0f);
    light.position_in_vs = VM * light.position;

    glBindBuffer(GL_UNIFORM_BUFFER, pvm_buffer_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &P[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &VM[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec4), &N[0]);//mat4
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec4), sizeof(glm::vec3), &N[1]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + 2 * sizeof(glm::vec4), sizeof(glm::vec3), &N[2]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBuffer(GL_UNIFORM_BUFFER, light_buffer);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &light.position_in_vs);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec4), &light.color);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), sizeof(glm::vec4), &light.a);
    glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4), sizeof(glm::vec3), &light.ambient);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBuffer(GL_UNIFORM_BUFFER, material_buffer);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(PhongMaterial), quad->getMaterial());
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    auto material = quad->getMaterial();
    if(material->Kd_map>0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material->Kd_map);
    }
    if(material->Ks_map>0) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, material->Ks_map);
    }
    if(material->Ns_map>0) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, material->Ns_map);
    }

    quad->draw();
    glBindVertexArray(0);
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0,0,w,h);
    camera_->perspective(glm::pi<float>()/4.0, (float)w/h, 0.1f, 100.0f);
}

void SimpleShapeApplication::mouse_button_callback(int button, int action, int mods) {
    Application::mouse_button_callback(button, action, mods);

    if (controler_) {
        double x, y;
        glfwGetCursorPos(window_, &x, &y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            controler_->LMB_pressed(x, y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
            controler_->LMB_released(x, y);
    }

}

void SimpleShapeApplication::cursor_position_callback(double x, double y) {
    Application::cursor_position_callback(x, y);
    if (controler_) {
        controler_->mouse_moved(x, y);
    }
}
