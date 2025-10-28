//
// Created by kristina on 10/28/25.
//

#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP
#include <engine/core/Controller.hpp>

namespace app {
    class MainController : public engine::core::Controller {
        void initialize() override;

        bool loop() override;

        void draw_saturn();

        void begin_draw() override;

        void end_draw() override;

        void draw() override;
    };
}

#endif //MAINCONTROLLER_HPP
