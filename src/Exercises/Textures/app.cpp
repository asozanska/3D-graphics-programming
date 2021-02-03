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

    pyramid = new Pyramid;

    auto u_transformations_index = glGetUniformBlockIndex(program, "Transformations");
    if (u_transformations_index == GL_INVALID_INDEX) {
        std::cout << "Cannot find Transformations uniform block in program" << std::endl;
    } else {
        glUniformBlockBinding(program, u_transformations_index, 0);
    }
    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    int w, h;
    std::tie(w, h) = frame_buffer_size();

    camera_->perspective(glm::pi<float>()/4.0, (float)w/h, 0.1f, 100.0f);
    camera_->look_at(glm::vec3(4.0f, -0.5f, -0.8f),
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

    auto  u_diffuse_map_location = glGetUniformLocation(program,"diffuse_map");
    if(u_diffuse_map_location==-1) {
        std::cerr<<"Cannot find uniform diffuse_map\n";
    } else {
        glUniform1ui(u_diffuse_map_location,0);
    }
}

void SimpleShapeApplication::frame() {
    auto PVM = camera_->projection() * camera_->view();
    pyramid->draw(PVM, pvm_buffer_);
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
