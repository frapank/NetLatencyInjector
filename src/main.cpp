#include <unistd.h>

#include "../include/gui.hpp"
#include "../include/utils.hpp"

int main() {
    Utils::checkIfSudo();
    Gui gui;

    while (true) {
        gui.drawGui();
    }

    return 0;
}
