#include <unistd.h>

#include "../include/Gui.hpp"
#include "../include/Utils.hpp"

int main() {
    Utils::checkIfSudo();
    Gui gui;

    while (true) {
        gui.drawGui();
    }

    return 0;
}
