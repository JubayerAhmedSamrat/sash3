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
  while (true)
  {
    std::string line;
    print_prompt();
    
    if(!std::getline(std::cin, line))
    {
      break;
    }
    auto tokens = lexer_.tokenize(line);
    Command command = parser_.parse(tokens);
    
   last_status_ = executor_.execute(command);
  }
}


void Shell::print_prompt()
{
  char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof(cwd)) != nullptr)
  {
    std::string path(cwd);
    auto pos = path.find_last_of('/');
    std::string current_dir;
    if (pos == std::string::npos)
    {
      current_dir = path;
    } else {
      current_dir = path.substr(pos + 1);
    }

    if (current_dir.empty())
    {
      current_dir = "/";
    }
    std::cout
      << "[~/sash: "
      << current_dir
      << (last_status_ == 0 ? " ✓" : " ✗")
      <<"]$ ";
  } else {
    std::cout<<"sash> ";
  }
}
