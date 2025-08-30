#include "lagger/gui.hpp"

#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "lagger/utils.hpp"

void Gui::clearGui() { std::cout << "\033[2J\033[H" << std::flush; }

void Gui::removeDelay() {
    auto cmd = "sudo tc qdisc del dev " + interface_ + " root";
    auto res = utils::executeCommand(cmd.c_str());
    if (!res) {
        clearGui();
        std::cerr << color::red << res.error() << color::reset << "\n";
    }
}

std::string Gui::getDelay() {
    auto cmd =
        "tc qdisc show dev " + interface_ + " | grep -oP 'delay \\K[0-9.]+ms'";
    auto res = utils::executeCommand(cmd.c_str());
    if (!res || res->empty()) {
        return std::string(color::red) + "No delay" + color::reset;
    }
    return res.value();
}

void Gui::setDelay() {
    clearGui();

    int time;
    std::cout << "Delay: ";
    std::cin >> time;
    const auto cmd = "sudo tc qdisc add dev " + interface_ +
                     " root netem delay " + std::to_string(time) + "ms";

    auto res = utils::executeCommand(cmd.c_str());
    if (!res) {
        removeDelay();
        std::cerr << color::red << res.error() << color::reset << "\n";
    } else {
        std::cout << color::yellow << "New delay set\n" << color::reset;
    }
}

std::expected<void, std::string> Gui::setupInterface() {
    clearGui();

    FILE* pipe{popen("ip -o link show | awk -F': ' '{print $2}'", "r")};
    if (!pipe) {
        return std::unexpected("Failed to list interfaces");
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
        return std::unexpected("No network interfaces foudn");
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

    interface_ = interfaces[choice - 1];
    return {};
}

void Gui::start() {
    if (auto check = setupInterface(); !check) {
        throw std::runtime_error(check.error());
        return;
    }

    running = true;
    while (running) {
        drawGui();
    }
}

void Gui::drawGui() {
    // Draw gui
    clearGui();
    std::cout << color::yellow << "Lagger settings: \n \n" << color::reset;
    std::cout << color::blue << " Current delay: " << color::reset << getDelay()
              << "\n"
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
        return;
    }

    // Elaborat
    switch (option) {
        case 1:
            setDelay();
            break;
        case 2:
            removeDelay();
            break;
        case 3:
            clearGui();
            running = false;
            break;
    }
}

