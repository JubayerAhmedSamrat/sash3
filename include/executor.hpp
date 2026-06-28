#pragma once
#include "builtin.hpp"
#include <vector>
#include <string>

class Executor
{
public:
  void execute(const std::vector<std::string>& tokens);

private:
  BuiltinManager builtin_;
};
