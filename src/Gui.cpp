#include "../include/Gui.hpp"

#include <iostream>
#include <cstdio>
#include <string>
#include <unistd.h>

#define RED    "\033[38;2;255;105;97m" 
#define GREEN  "\033[38;2;119;221;119m"
#define YELLOW "\033[38;2;253;253;150m" 
#define BLUE   "\033[38;2;174;198;207m"
#define RESET  "\033[0m"

void clearGui(){
  system("clear");
}

void removeDelay(){
  std::string cmd = "sudo tc qdisc del dev enp42s0 root";
  int result = system(cmd.c_str());
  if(result != 0){
    std::cout << RED << "Error in esecution \n" << RESET;
    exit(1);
  }
}

void firstInput(){

  clearGui();

  int time;
  std::cout << "Delay: ";
  std::cin >> time;
  std::string cmd = "sudo tc qdisc add dev enp42s0 root netem delay " + std::to_string(time) + "ms";

  int result;
  do {
    result = system(cmd.c_str());
    if (result != 0) {
      removeDelay();
    }
  } while (result != 0);

  std::cout << YELLOW << "New delay: "<< result << "\n" << RESET;

}

void secondInput(){

  clearGui();

  removeDelay();
  std::cout << YELLOW << "Delay removed! \n" << RESET;

  clearGui();
}

std::string getDelay() {

    const char* cmd = "tc qdisc show dev enp42s0 | grep -oP 'delay \\K[0-9.]+ms'";
    FILE* pipe = popen(cmd, "r");
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

void Gui::drawGui(){

  clearGui();

  std::cout << YELLOW <<"Lagger settings: \n \n" << RESET;

  std::cout << BLUE << " Current delay: " << RESET << getDelay() << "\n" << RESET;
  std::cout << GREEN << " 1) Set new delay \n" << RESET;
  std::cout << GREEN << " 2) Remove current delay \n" << RESET;
  std::cout << RED << " 3) Exit\n" << RESET;

  int option;
  std::cout << "\n\n" << YELLOW << "Input: " << RESET;
  if(!(std::cin >> option)){
    std::cin.clear();
    std::cin.ignore(1000, '\n');
    drawGui();
    return;
  }

  switch(option){
    case 1:{

      firstInput();	
      break;

    }
    case 2:{

      secondInput();
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

