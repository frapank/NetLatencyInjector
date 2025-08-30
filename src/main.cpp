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

    try {
        Gui gui;
        gui.start();
    } catch (const std::runtime_error& e) {
        std::cerr << color::red << "Critical Error: " << e.what()
                  << color::reset << "\n";
        return EXIT_FAILURE;
    } catch (const std::exception& e) {
        std::cerr << color::red << "Unexpected Error: " << e.what()
                  << color::reset << "\n";
        return EXIT_FAILURE;
    }
}
