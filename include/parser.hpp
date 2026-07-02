#pragma once

#include "command.hpp"
#include <vector>
#include <string>

class Parser
{
public:
  Command parse(const std::vector<std::string>& tokens);
};
