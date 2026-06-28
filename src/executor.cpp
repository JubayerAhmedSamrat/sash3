#include "executor.hpp"
#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void Executor::execute(
    const std::vector<std::string>& tokens)
{
  if(tokens.empty())
  {
    return;
  }
  if(tokens[0] == "exit")
  {
    std::exit(0);
  }

  pid_t pid = fork();

  if(pid < 0)
  {
    perror("fork");
    return;
  }
  if(pid == 0)
  {
  //child porcess---------
    std::vector<char*> argv;

    for(const auto& token : tokens)
    {
      argv.push_back(
          const_cast<char*>(token.c_str())
          );
    }

    argv.push_back(nullptr);

    execvp(argv[0], argv.data());
    perror("execvp");
    std::exit(EXIT_FAILURE);

  } else {
  //parent process--------
    int status;
    waitpid(pid, &status, 0);
  }

}
