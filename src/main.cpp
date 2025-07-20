#include <unistd.h>
#include <unistd.h>

#include "../include/Utils.hpp"
#include "../include/Gui.hpp"

int main(){

  Utils::checkIfSudo();
  Gui gui;

  while(true){
    gui.drawGui();
  }

  return 0;

}
