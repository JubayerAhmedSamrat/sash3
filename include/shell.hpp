#pragma once

#include "executor.hpp"
#include "lexer.hpp"
class Shell
{
  public:
    void run();

  private:
    void loop();
  
    Lexer lexer_;
    Executor executor_;
};
