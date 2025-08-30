#include "lagger/utils.hpp"

#include <unistd.h>

#include <cstdio>
#include <filesystem>
#include <memory>
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
    auto pipeCloser = [](FILE* f) {
        if (f) pclose(f);
    };

    std::unique_ptr<FILE, decltype(pipeCloser)> pipe(popen(command, "r"),
                                                     pipeCloser);

    if (!pipe) {
        return std::unexpected(
            std::string{"Can't open pipe for: " + std::string(command) + '\n'});
    }

    std::string output;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe.get())) output += buffer;

    return output;
}

bool checkIfSudo() { return getuid() == 0; }

}  // namespace utils
