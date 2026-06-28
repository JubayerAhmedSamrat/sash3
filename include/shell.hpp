#pragma once

#include "executor.hpp"
#include "lexer.hpp"
class Shell
{
  public:
    void run();

  private:
    void loop();
    void print_prompt();
  
    Lexer lexer_;
    Executor executor_;
};
