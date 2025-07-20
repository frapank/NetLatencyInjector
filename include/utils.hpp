#pragma once

#include <string>

class Utils {
   public:
    void static checkIfSudo();
    std::string static executeCommand(const char* command);
};
