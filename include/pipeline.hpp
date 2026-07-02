#pragma once

#include "command.hpp"
#include <vector>

struct Pipeline
{
  std::vector<Command> commands;

  bool background = false;
};
