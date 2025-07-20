#include "../include/Gui.hpp"

#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#define RED "\033[38;2;255;105;97m"
#define GREEN "\033[38;2;119;221;119m"
#define YELLOW "\033[38;2;253;253;150m"
#define BLUE "\033[38;2;174;198;207m"
#define RESET "\033[0m"

void clearGui() { system("clear"); }

void removeDelay(std::string interface) {
    std::string cmd = "sudo tc qdisc del dev " + interface + " root";
    int result = system(cmd.c_str());
    if (result != 0) {
        clearGui();
    }
}

void firstInput(std::string interface) {
    clearGui();

    int time;
    std::cout << "Delay: ";
    std::cin >> time;
    std::string cmd = "sudo tc qdisc add dev " + interface +
                      " root netem delay " + std::to_string(time) + "ms";

    int result;
    do {
        result = system(cmd.c_str());
        if (result != 0) {
            removeDelay(interface);
        }
    } while (result != 0);

    std::cout << YELLOW << "New delay: " << result << "\n" << RESET;
}

void secondInput(std::string interface) {
    clearGui();

    removeDelay(interface);
    std::cout << YELLOW << "Delay removed! \n" << RESET;

    clearGui();
}

std::string getDelay(std::string interface) {
    std::string cmd =
        "tc qdisc show dev " + interface + " | grep -oP 'delay \\K[0-9.]+ms'";
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        std::cout << RED << "Can't open pipe \n" << RESET;
        exit(-1);
    }

    char buffer[256];
    std::string result;

    while (fgets(buffer, sizeof(buffer), pipe)) {
        result += buffer;
    }

    pclose(pipe);

    if (result.empty()) {
        return RED + std::string("No delay") + RESET;
    } else {
        return result;
    }
}

char* checkInterface() {
    clearGui();

    FILE* pipe = popen("ip -o link show | awk -F': ' '{print $2}'", "r");
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

    std::cout << YELLOW << "\nAvailable network interfaces:\n" << RESET;
    for (size_t i = 0; i < interfaces.size(); ++i) {
        std::cout << GREEN << " " << i + 1 << ") " << interfaces[i] << "\n"
                  << RESET;
    }

    int choice = 0;
    while (true) {
        std::cout << YELLOW << "\nChoose interface (1-" << interfaces.size()
                  << "): " << RESET;
        std::cin >> choice;
        if (std::cin.fail() || choice < 1 ||
            (size_t)choice > interfaces.size()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid choice. Try again.\n";
        } else
            break;
    }

    char* selected = new char[interfaces[choice - 1].size() + 1];
    std::strcpy(selected, interfaces[choice - 1].c_str());
    return selected;
}

Gui::Gui() { this->interface = checkInterface(); }

void Gui::drawGui() {
    // Draw gui
    clearGui();
    std::cout << YELLOW << "Lagger settings: \n \n" << RESET;
    std::cout << BLUE << " Current delay: " << RESET << getDelay(interface)
              << "\n"
              << RESET;
    std::cout << GREEN << " 1) Set new delay \n" << RESET;
    std::cout << GREEN << " 2) Remove current delay \n" << RESET;
    std::cout << RED << " 3) Exit\n" << RESET;

    // Select option
    int option;
    std::cout << "\n\n" << YELLOW << "Input: " << RESET;
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

