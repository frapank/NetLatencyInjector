#include <cstdio>
#include <unistd.h>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <cstring>

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
