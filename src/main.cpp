#include <unistd.h>

#include "../include/gui.hpp"
#include "../include/utils.hpp"

int main() {
    utils::checkIfSudo();
    utils::checkTcInstalled();
    Gui gui;

    while (true) {
        gui.drawGui();
    }

    return 0;
}
