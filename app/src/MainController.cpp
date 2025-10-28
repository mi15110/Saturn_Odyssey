//
// Created by kristina on 10/28/25.
//
#include <MainController.hpp>
#include <engine/platform/PlatformController.hpp>
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
        engine::resources::Model *saturn = resources->model("saturn");
        engine::resources::Shader *shader = resources->shader("basic");
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));

        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());
        shader->set_mat4("model", model);

        saturn->draw(shader);
    }

    void MainController::begin_draw() {
        engine::graphics::OpenGL::clear_buffers();
    }

    void MainController::end_draw() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        platform->swap_buffers();
    }

    void MainController::draw() {
        draw_saturn();
    }
}
