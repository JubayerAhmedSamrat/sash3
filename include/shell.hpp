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

    int last_status_ = 0;
  
    Lexer lexer_;
    Executor executor_;
};
