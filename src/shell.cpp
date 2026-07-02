#include "shell.hpp"
#include "pipeline.hpp"
#include <iostream>
#include <string>
#include <unistd.h>
#include <limits.h>

constexpr const char* RESET = "\033[0m";
constexpr const char* CYAN = "\033[36m";
constexpr const char* BLUE = "\033[34m";
constexpr const char* GREEN = "\033[32m";
constexpr const char* RED = "\033[31m";
constexpr const char* GRAY = "\033[90m";
constexpr const char* BOLD = "\033[1m";

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
    Pipeline pipeline = parser_.parse(tokens);
    
   last_status_ = executor_.execute(pipeline);
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
      <<BOLD
      << CYAN
      << "sash"
      << RESET
      << '\n'
      << GRAY
      << "└─ "
      << RESET
      << current_dir
      << ' '
      << (last_status_ == 0 ? GREEN : RED)
      << (last_status_ == 0 ? " ✓" : " ✗")
      << RESET
      << " $ ";
  } else {
    std::cout<<"sash> ";
  }
}
