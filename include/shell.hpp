#pragma once

#include "executor.hpp"
class Shell
{
  public:
    void run();

  private:
    void loop();

    Executor executor_;
};
