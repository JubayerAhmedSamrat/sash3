#pragma once

#include "parser.hpp"
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
    Parser parser_;
    Executor executor_;
};
