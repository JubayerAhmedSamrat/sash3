#pragma once
#include "Pipeline.hpp"
#include "command.hpp"
#include <vector>
#include <string>

class Parser
{
public:
  Pipeline parse(const std::vector<std::string>& tokens);
};
