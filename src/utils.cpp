#include "lagger/utils.hpp"

#include <unistd.h>

#include <cstdio>
#include <filesystem>
#include <vector>

namespace utils {

std::expected<void, std::string> checkTcInstalled() {
    const std::vector<std::string> paths = {
        "/usr/bin/", "/usr/sbin/",      "/bin/",
        "/sbin/",    "/usr/local/bin/", "/usr/local/sbin/"};

    for (const auto& dir : paths) {
        auto candidate = std::filesystem::path(dir) / "tc";
        if (std::filesystem::exists(candidate) &&
            std::filesystem::is_regular_file(candidate)) {
            return {};
        }
    }
    return std::unexpected("iproute-tc not installed");
}

std::expected<std::string, std::string> executeCommand(const char* command) {
    FILE* pipe{popen(command, "r")};
    std::string output;

    if (!pipe) {
        return std::unexpected(
            std::string{"Can't open pipe for: " + std::string(command) + '\n'});
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe)) {
        output.append(buffer);
    }

    int ret{pclose(pipe)};
    if (ret == -1) {
        return std::unexpected(std::string{
            "Can't close the pipe: " + std::string(command) + '\n'});
    }

    return output;
}

bool checkIfSudo() { return getuid() == 0; }
}  // namespace utils
