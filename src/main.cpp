#include <iostream>
#include <cstdio>
#include <string>
#include <unistd.h>

#include "../include/Utils.hpp"
#include "../include/Gui.hpp"



int main(){

  Utils::checkIfSudo();

  while(true){
    Gui::drawGui();
  }


  return 0;

}
