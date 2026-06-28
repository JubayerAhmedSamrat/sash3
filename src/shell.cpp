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
    auto tokens = lexer_.tokenize(line);
    
    for(std::size_t i = 0; i < tokens.size(); ++i)
    {
      std::cout 
        <<"Token "
        << i 
        << ": "
        << tokens[i]
        <<'\n';
    }
    //executor_.execute(line);
  }
}
