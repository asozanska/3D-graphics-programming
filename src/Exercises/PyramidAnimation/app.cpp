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

    pyramid_ = std::make_shared<Pyramid>();

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
    camera_->look_at(glm::vec3(0.0f, 25.0f, 0.0f),
                     glm::vec3(0.0f, 0.3f, 0.5f),
                     glm::vec3(0.0f, 0.5f, 0.0f));

    start_ = std::chrono::steady_clock::now();
    rotation_period = 4.0;

    glGenBuffers(1, &pvm_buffer_);
    glBindBuffer(GL_UNIFORM_BUFFER, pvm_buffer_);
    glBufferData(GL_UNIFORM_BUFFER, 4 * 4 * sizeof(float), nullptr, GL_STATIC_DRAW);

    glViewport(0, 0, w, h);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);
}

void SimpleShapeApplication::frame() {
    auto now = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(now - start_).count();
    auto rotation_angle = 2.0f * glm::pi<float>() * elapsed_time/rotation_period;
    auto R = glm::rotate(glm::mat4(1.0f), rotation_angle,glm::vec3{0.0f, 1.0f, 0.0f});

    auto orbital_rotation_period = 20.0f;
    auto orbital_rotation_angle = 2.0f * glm::pi<float>() * elapsed_time / orbital_rotation_period;
    auto x = 10 * cos(orbital_rotation_angle);
    auto z = 8 * sin(orbital_rotation_angle);
    auto O = glm::translate(glm::mat4(1.0f), glm::vec3{x,0.0,z});
    auto M = camera_->projection() * camera_->view() * O * R;
    pyramid_->draw(M, pvm_buffer_);

    auto moon = glm::scale(glm::mat4(1.0f), glm::vec3{0.5f, 0.5f, 0.5f});
    auto moon_rotation_angle = 2.0f * glm::pi<float>() * elapsed_time / 10.0f;
    auto R_moon =  glm::rotate(glm::mat4(1.0f), moon_rotation_angle, glm::vec3{0.0f, 1.0f, 0.0f});
    auto moon_orbital_rotation_angle = -(2.0f * glm::pi<float>() * elapsed_time / 10.0f);
    auto O_moon= glm::translate(glm::mat4(1.0f), glm::vec3{(3.0f * cos(moon_orbital_rotation_angle)),0.0f, (3.0f * sin(moon_orbital_rotation_angle))});
    auto M_moon = camera_->projection() * camera_->view() * O * O_moon * R_moon * moon;
    pyramid_->draw(M_moon, pvm_buffer_);

    auto satellite = glm::scale(glm::mat4(1.0f), glm::vec3{0.25f, 0.25f, 0.25f});
    auto satellite_rotation_angle = 2.0f * glm::pi<float>() * elapsed_time / 2.0f;
    auto R_satellite =  glm::rotate(glm::mat4(1.0f), satellite_rotation_angle, glm::vec3{0.0f, 0.0f, 1.0f});
    auto satellite_orbital_rotation_angle = 2.0f * glm::pi<float>() * elapsed_time / 2.0f;
    auto O_satellite= glm::translate(glm::mat4(1.0f), glm::vec3{(1.5f * cos(satellite_orbital_rotation_angle)),(1.5f * sin(satellite_orbital_rotation_angle)),0.0f});
    auto M_satellite = camera_->projection() * camera_->view() * O * O_satellite * R_satellite * satellite;
    pyramid_->draw(M_satellite, pvm_buffer_);

    glBindVertexArray(0);
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0,0,w,h);
    camera_->set_aspect((float)w/h);
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
