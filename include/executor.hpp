#pragma once
#include "builtin.hpp"
#include <vector>
#include <string>

class Executor
{
public:
  int execute(const std::vector<std::string>& tokens);

private:
  BuiltinManager builtin_;
};
