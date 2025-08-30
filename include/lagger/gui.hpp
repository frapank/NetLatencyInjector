#pragma once

#include <string>
#include <expected>

namespace color {
constexpr const char* red = "\033[38;2;255;105;97m";
constexpr const char* green = "\033[38;2;119;221;119m";
constexpr const char* yellow = "\033[38;2;253;253;150m";
constexpr const char* blue = "\033[38;2;174;198;207m";
constexpr const char* reset = "\033[0m";
}  // namespace color

class Gui {
   public:
    void start();

   private:
    std::string getDelay();
    std::expected<void, std::string> setupInterface();
    void removeDelay();
    void setDelay();
    void clearGui();
    void drawGui();

    std::string interface_{};
    bool running = false;
};
