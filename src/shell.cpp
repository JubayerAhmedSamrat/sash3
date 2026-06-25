#include "shell.hpp"
#include<iostream>
#include <string>

void Shell::run()
{
  loop();
}

void Shell::loop()
{
  std::string line;

  std::cout << "sash3\n";
  std::getline(std::cin, line);

  std::cout<< "You typed: " << line << '\n';
}
