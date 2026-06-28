#include "shell.hpp"
#include <iostream>
#include <string>

void Shell::run()
{
  loop();
}

void Shell::loop()
{
  while(true)
  {
    std::string line;
    std::cout << "sash3 ";
    
    if(!std::getline(std::cin, line))
    {
      break;
    }
    executor_.execute(line);
  }
}
