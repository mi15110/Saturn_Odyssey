//
// Created by kristina on 10/28/25.
//

#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP
#include <engine/core/Controller.hpp>

#include "glm/vec3.hpp"

namespace app {
    inline glm::vec3 m_sunPos{4.5f, 2.0f, -3.0f};

    class MainController : public engine::core::Controller {
        void initialize() override;

        bool loop() override;

        void draw_saturn();

        void draw_sun();

        void draw_skybox();

        void update_camera();

        void update() override;

        void begin_draw() override;

        void end_draw() override;

        void draw() override;
    };
}

#endif //MAINCONTROLLER_HPP
