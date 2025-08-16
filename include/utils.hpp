#pragma once

#include <string>

namespace utils {
void checkIfSudo();
std::string executeCommand(const char* command);
}  // namespace utils
