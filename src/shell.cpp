#include "shell.hpp"
#include <iostream>
#include <string>
#include <unistd.h>
#include <limits.h>

void Shell::run()
{
  loop();
}

void Shell::loop()
{
  while(true)
  {
    std::string line;
    print_prompt();
    
    if(!std::getline(std::cin, line))
    {
      break;
    }
    auto tokens = lexer_.tokenize(line);
    
    executor_.execute(tokens);
  }
}


void Shell::print_prompt()
{
  char cwd[PATH_MAX];
  if(getcwd(cwd, sizeof(cwd)) != nullptr)
  {
    std::cout<< "[~/sash: "<< cwd <<"] ";
  } else {
    std::cout<<"sash> ";
  }
}
