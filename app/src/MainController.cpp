//
// Created by kristina on 10/28/25.
//
#include <MainController.hpp>
#include <engine/platform/PlatformController.hpp>
#include <GL/gl.h>
#include <spdlog/spdlog.h>

#include "engine/graphics/GraphicsController.hpp"
#include "engine/graphics/OpenGL.hpp"
#include "engine/resources/ResourcesController.hpp"

namespace app {
    void MainController::initialize() {
        spdlog::info("Kontroler inicijalizovan");
        engine::graphics::OpenGL::enable_depth_testing();
    }

    bool MainController::loop() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KEY_ESCAPE).is_down()) { return false; }

        return true;
    }

    void MainController::draw_saturn() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics = engine::graphics::GraphicsController::get<engine::graphics::GraphicsController>();
        auto camera = graphics->camera();

        engine::resources::Model *saturn = resources->model("saturn");
        engine::resources::Shader *shader = resources->shader("saturn_shader");

        shader->use();

        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", camera->view_matrix());

        glm::mat4 modelSat = glm::mat4(1.0f);
        modelSat = glm::translate(modelSat, glm::vec3(0.0f, -0.1f, -4));
        modelSat = glm::scale(modelSat, glm::vec3(1.2f));


        shader->set_mat4("model", modelSat);

        resources->texture("saturn")->bind(GL_TEXTURE0);
        shader->set_int("texture_sphere", 0);

        glm::vec3 lightDir = glm::normalize(glm::vec3(0.0f) - m_sunPos);
        shader->set_vec3("lightDirection", lightDir);
        shader->set_vec3("viewPos", camera->Position);
        shader->set_vec3("lightColor", glm::vec3(1.0f, 0.95f, 0.85f));
        saturn->draw(shader);
    }
}

void app::MainController::draw_sun() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

    engine::resources::Model *sun = resources->model("sun");
    engine::resources::Shader *shader = resources->shader("sun_shader");

    m_sunPos = glm::vec3(2.5f, 0.8f, -0.6f);

    glm::mat4 modelSun = glm::mat4(1.0f);
    modelSun = glm::translate(modelSun, m_sunPos);
    modelSun = glm::scale(modelSun, glm::vec3(0.3f));

    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    shader->set_mat4("model", modelSun);

    resources->texture("sun")->bind(GL_TEXTURE0);
    shader->set_int("texture_sun", 0);

    sun->draw(shader);
}


void app::MainController::update_camera() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    auto graphics = engine::graphics::GraphicsController::get<engine::graphics::GraphicsController>();
    auto camera = graphics->camera();
    float dt = platform->dt();

    if (platform->key(engine::platform::KeyId::KEY_W).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::FORWARD, dt);
    }

    if (platform->key(engine::platform::KeyId::KEY_S).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, dt);
    }

    if (platform->key(engine::platform::KeyId::KEY_A).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::LEFT, dt);
    }

    if (platform->key(engine::platform::KeyId::KEY_D).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::RIGHT, dt);
    }
}

void app::MainController::update() {
    update_camera();
}

void app::MainController::begin_draw() {
    engine::graphics::OpenGL::clear_buffers();
}

void app::MainController::end_draw() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->swap_buffers();
}

void app::MainController::draw() {
    draw_saturn();
    draw_sun();
}

