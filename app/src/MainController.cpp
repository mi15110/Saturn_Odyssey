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
    class MainPlatformEventObserver : public engine::platform::PlatformEventObserver {
    public:
        void on_mouse_move(engine::platform::MousePosition position) override;
    };

    void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
        auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
        camera->rotate_camera(position.dx, position.dy);
    }

    void MainController::initialize() {
        spdlog::info("Kontroler inicijalizovan");
        auto graphics = engine::graphics::GraphicsController::get<engine::graphics::GraphicsController>();
        auto camera = graphics->camera();
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());

        engine::graphics::OpenGL::enable_depth_testing();
        camera->Position = glm::vec3(0.0f, 0.3f, 8.0f);
        camera->Front = glm::normalize(glm::vec3(0.0f, 0.0f, -5.0f) - camera->Position);
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
        shader->set_mat4("view", camera->view_matrix());;

        glm::mat4 modelSat = glm::mat4(1.0f);
        modelSat = glm::translate(modelSat, glm::vec3(0.0f, -0.2f, -5.0f));
        modelSat = glm::scale(modelSat, glm::vec3(1.5f));
        shader->set_mat4("model", modelSat);

        resources->texture("saturn")->bind(GL_TEXTURE0);
        shader->set_int("texture_sphere", 0);

        glm::vec3 lightDir = glm::normalize(glm::vec3(0.0f) - m_sunPos);
        shader->set_vec3("lightDirection", lightDir);
        shader->set_vec3("viewPos", camera->Position);
        shader->set_vec3("lightColor", glm::vec3(1.0f, 0.95f, 0.85f));
        saturn->draw(shader);
    }

    void MainController::draw_sun() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

        engine::resources::Model *sun = resources->model("sun");
        engine::resources::Shader *shader = resources->shader("sun_shader");

        m_sunPos = glm::vec3(2.8f, 0.8f, -0.6f);

        glm::mat4 modelSun = glm::mat4(1.0f);
        modelSun = glm::translate(modelSun, m_sunPos);
        modelSun = glm::scale(modelSun, glm::vec3(0.4f));

        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());
        shader->set_mat4("model", modelSun);

        resources->texture("sun")->bind(GL_TEXTURE0);
        shader->set_int("texture_sun", 0);

        sun->draw(shader);
    }

    void MainController::draw_skybox() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto skybox = resources->skybox("skybox");
        auto shader = resources->shader("skybox_shader");
        auto graphics = engine::graphics::GraphicsController::get<engine::graphics::GraphicsController>();
        graphics->draw_skybox(shader, skybox);
    }


    void MainController::update_camera() {
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

    void MainController::update() {
        update_camera();
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
        draw_sun();
        draw_skybox();
    }
}
