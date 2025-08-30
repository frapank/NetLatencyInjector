#pragma once

#include <expected>
#include <string>

namespace utils {

std::expected<void, std::string> checkTcInstalled();
std::expected<std::string, std::string> executeCommand(const char* command);
bool checkIfSudo();

}  // namespace utils
