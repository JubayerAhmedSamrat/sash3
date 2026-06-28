#include "executor.hpp"
#include <cstdlib>
void Executor::execute(const std::string& command)
{
  if(command == "exit")
  {
    std::exit(0);
  }
}
