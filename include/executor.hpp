#pragma once
#include "builtin.hpp"
#include "command.hpp"
#include <vector>
#include <string>

class Executor
{
public:
  int execute(const Command& command);

private:
  BuiltinManager builtin_;
};
