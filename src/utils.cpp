#include "../include/utils.hpp"

#include <unistd.h>

#include <cstdio>
#include <iostream>
#include <string>

namespace utils {
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
