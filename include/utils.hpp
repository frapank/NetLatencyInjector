#pragma once

#include <string>

namespace utils {
bool checkTcInstalled();
void checkIfSudo();
std::string executeCommand(const char* command);
}  // namespace utils
