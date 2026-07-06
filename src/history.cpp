#include "history.hpp"
#include <iostream>

void History::add(const std::string& command)
{
  commands_.push_back(command);
}

void History::print() const 
{
  for(size_t i = 0; i < commands_.size(); ++i)
  {
    std::cout 
      << i + 1 
      << " "
      << commands_[i]
      << '\n';
  }
}
