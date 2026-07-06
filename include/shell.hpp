#pragma once

#include "jobs.hpp"
#include "parser.hpp"
#include "executor.hpp"
#include "lexer.hpp"
class Shell
{
  public:

    Shell();
    void run();

  private:
    void loop();
    void print_prompt();
    void reapBackgroundJobs();

    int last_status_ = 0;
  
    Lexer lexer_;
    Parser parser_;
    JobsManager jobs_;
    Executor executor_;
};
