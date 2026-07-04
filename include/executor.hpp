#pragma once
#include "builtin.hpp"
#include "pipeline.hpp"
#include <vector>
#include <string>

class Executor
{
public:
  int execute(const Pipeline& pipeline);

private:
  int executeSingle(const Command& command, bool background);
  void setupRedirection(const Command& command);


  [[noreturn]]
  void execCommand(const Command& command);

  BuiltinManager builtin_;
};
