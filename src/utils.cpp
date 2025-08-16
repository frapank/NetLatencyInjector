#include "../include/utils.hpp"

#include <unistd.h>

#include <cstdio>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace utils {
bool checkTcInstalled() {
    std::vector<std::string> paths = {"/usr/bin/",       "/usr/sbin/",
                                      "/bin/",           "/sbin/",
                                      "/usr/local/bin/", "/usr/local/sbin/"};

    for (const auto& dir : paths) {
        std::filesystem::path candidate = dir + std::string("tc");
        if (std::filesystem::exists(candidate) &&
            std::filesystem::is_regular_file(candidate)) {
            return true;
        }
    }
    std::cerr << "Error: iproute-tc not installed" << "\n";
    exit(1);
}

std::string executeCommand(const char* command) {
    FILE* pipe = popen(command, "r");
    std::string output;

    if (!pipe) {
        std::cerr << "Can't open pipe for: " << command << "\n";
        exit(-1);
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe)) {
        output.append(buffer);
    }

    int ret = pclose(pipe);
    if (ret == -1) {
        std::cerr << "Can't close the pipe \n";
        exit(-1);
    }

    return output;
}

void checkIfSudo() {
    if (geteuid() != 0) {
        std::cout << "You need to use sudo for this program\n";
        exit(0);
    }
}
}  // namespace utils
