#include "shell.hpp"
#include "pipeline.hpp"
#include <iostream>
#include <string>
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>

constexpr const char* RESET = "\033[0m";
constexpr const char* CYAN = "\033[36m";
constexpr const char* BLUE = "\033[34m";
constexpr const char* GREEN = "\033[32m";
constexpr const char* RED = "\033[31m";
constexpr const char* GRAY = "\033[90m";
constexpr const char* BOLD = "\033[1m";

void Shell::run()
{
  loop();
}

void Shell::loop()
{
  while (true)
  {
    reapBackgroundJobs();

    std::string line;
    print_prompt();
    
    if(!std::getline(std::cin, line))
    {
      if(std::cin.eof())
      {
        break;
      }

      std::cin.clear();
      std::cout<< '\n';
      continue;
    }
    auto tokens = lexer_.tokenize(line);

    if(tokens.size() == 1 && tokens[0] == "jobs")
    {
      executor_.printJobs();
      continue;
    }
    if(tokens.size() == 1 && tokens[0] == "fg")
    {
      executor_.foregroundLastJob();
      continue;
    }

    if(tokens.size() == 1 && tokens[0] == "bg")
    {
      executor_.backgroundLastJob();
      continue;
    }

  Pipeline pipeline = parser_.parse(tokens);  
  last_status_ = executor_.execute(pipeline);
  }
}

void Shell::reapBackgroundJobs()
{
  int status;
  pid_t pid;

  while((pid = waitpid(-1, &status, WNOHANG)) > 0)
  {
    executor_.removeJob(pid);
  }
}
void Shell::print_prompt()
{
  char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof(cwd)) != nullptr)
  {
    std::string path(cwd);
    auto pos = path.find_last_of('/');
    std::string current_dir;
    if (pos == std::string::npos)
    {
      current_dir = path;
    } else {
      current_dir = path.substr(pos + 1);
    }

    if (current_dir.empty())
    {
      current_dir = "/";
    }
    std::cout
      <<BOLD
      << CYAN
      << "sash"
      << RESET
      << '\n'
      << GRAY
      << "└─ "
      << RESET
      << current_dir
      << ' '
      << (last_status_ == 0 ? GREEN : RED)
      << (last_status_ == 0 ? " ✓" : " ✗")
      << RESET
      << " $ ";
  } else {
    std::cout<<"sash> ";
  }
}

Shell::Shell()
  :executor_(jobs_)
{

}
