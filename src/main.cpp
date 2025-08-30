#include <unistd.h>

#include <iostream>
#include <stdexcept>

#include "lagger/gui.hpp"
#include "lagger/utils.hpp"

int main() {
    if (!utils::checkIfSudo()) {
        std::cerr << "The program should run as sudo\n";
        return EXIT_FAILURE;
    }

    if (auto check = utils::checkTcInstalled(); !check) {
        std::cerr << check.error() << "\n";
        return EXIT_FAILURE;
    }

    Gui gui;
    while (true) {
        gui.drawGui();
    }
}
