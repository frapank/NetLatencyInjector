#include "lagger/gui.hpp"

#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "lagger/utils.hpp"

void clearGui() { std::system("clear"); }

void removeDelay(const std::string& interface) {
    auto cmd = "sudo tc qdisc del dev " + interface + " root";
    auto res = utils::executeCommand(cmd.c_str());
    if (!res) {
        clearGui();
        std::cerr << color::red << res.error() << color::reset << "\n";
    }
}

std::string getDelay(const std::string& interface) {
    auto cmd = "tc qdisc show dev " + interface +
               " | grep -oP 'delay \\K[0-9.]+ms'";
    auto res = utils::executeCommand(cmd.c_str());
    if (!res || res->empty()) {
        return std::string(color::red) + "No delay" + color::reset;
    }
    return res.value();
}

void firstInput(std::string interface) {
    clearGui();

    int time;
    std::cout << "Delay: ";
    std::cin >> time;
    const auto cmd = "sudo tc qdisc add dev " + interface +
                     " root netem delay " + std::to_string(time) + "ms";

    auto res = utils::executeCommand(cmd.c_str());
    if (!res) {
        removeDelay(interface);
        std::cerr << color::red << res.error() << color::reset << "\n";
    } else {
        std::cout << color::yellow << "New delay set\n" << color::reset;
    }
}

void secondInput(std::string interface) {
    clearGui();

    removeDelay(interface);
    std::cout << color::yellow << "Delay removed! \n" << color::reset;

    clearGui();
}

char* checkInterface() {
    clearGui();

    FILE* pipe{popen("ip -o link show | awk -F': ' '{print $2}'", "r")};
    if (!pipe) {
        std::cerr << "Failed to list interfaces\n";
        exit(1);
    }

    std::vector<std::string> interfaces;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe)) {
        std::string iface(buffer);
        iface.erase(iface.find_last_not_of(" \n\r\t") + 1);
        if (iface != "lo") {
            interfaces.push_back(iface);
        }
    }
    pclose(pipe);

    if (interfaces.empty()) {
        std::cerr << "No network interfaces found.\n";
        exit(1);
    }

    std::cout << color::yellow << "\nAvailable network interfaces:\n"
              << color::reset;
    for (size_t i = 0; i < interfaces.size(); ++i) {
        std::cout << color::green << " " << i + 1 << ") " << interfaces[i]
                  << "\n"
                  << color::reset;
    }

    int choice = 0;
    while (true) {
        std::cout << color::yellow << "\nChoose interface (1-"
                  << interfaces.size() << "): " << color::reset;
        std::cin >> choice;
        if (std::cin.fail() || choice < 1 ||
            (size_t)choice > interfaces.size()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid choice. Try again.\n";
        } else
            break;
    }

    char* selected{new char[interfaces[choice - 1].size() + 1]};
    std::strcpy(selected, interfaces[choice - 1].c_str());
    return selected;
}

Gui::Gui() { this->interface = checkInterface(); }

void Gui::drawGui() {
    // Draw gui
    clearGui();
    std::cout << color::yellow << "Lagger settings: \n \n" << color::reset;
    std::cout << color::blue << " Current delay: " << color::reset
              << getDelay(interface) << "\n"
              << color::reset;
    std::cout << color::green << " 1) Set new delay \n" << color::reset;
    std::cout << color::green << " 2) Remove current delay \n" << color::reset;
    std::cout << color::red << " 3) Exit\n" << color::reset;

    // Select option
    int option;
    std::cout << "\n\n" << color::reset << "Input: " << color::reset;
    if (!(std::cin >> option)) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        drawGui();
        return;
    }

    // Elaborate option
    switch (option) {
        case 1: {
            firstInput(interface);
            break;
        }
        case 2: {
            secondInput(interface);
            break;
        }
        case 3: {
            clearGui();
            exit(0);
            break;
        }
    }

    drawGui();
}

