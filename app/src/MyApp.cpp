//
// Created by kristina on 10/27/25.
//

#include <MainController.hpp>
#include <MyApp.hpp>
#include <spdlog/spdlog.h>

namespace app {
void MyApp::app_setup() {
    spdlog::info("App setup!");
    auto main_controller = register_controller<app::MainController>();
}
}