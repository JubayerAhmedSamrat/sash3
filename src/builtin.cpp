#include "builtin.hpp"

#include <unistd.h>
#include <cstdlib>
#include <iostream>

bool BuiltinManager::execute(
    const std::vector<std::string>& tokens)
{
  if(tokens.empty())
  {
    return false;
  }

  if(tokens[0] != "cd")
  {
    return false;
  }

  const char* path = nullptr;

  if(tokens.size() == 1)
  {
    path = std::getenv("HOME");
  } else 
  {
    path = tokens[1].c_str();
  }

  if(chdir(path) != 0)
  {
    perror("cd");
  }

  return true;
}
