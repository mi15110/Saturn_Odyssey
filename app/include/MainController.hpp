//
// Created by kristina on 10/28/25.
//

#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP
#include <engine/core/Controller.hpp>

namespace app {
class MainController : public engine::core::Controller {
    void initialize() override;
};
}

#endif //MAINCONTROLLER_HPP