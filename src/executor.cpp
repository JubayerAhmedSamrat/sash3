#include "executor.hpp"
 
#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int Executor::execute(
    const std::vector<std::string>& tokens)
{
  if(tokens.empty())
  {
    return 0;
  }
  if(builtin_.execute(tokens))
  {
    return 0;
  }
  if(tokens[0] == "exit")
  {
    std::exit(0);
  }

  pid_t pid = fork();

  if(pid < 0)
  {
    perror("fork");
    return 1;
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

    if(WIFEXITED(status))
    {
      return WEXITSTATUS(status);
    }
    
    return 1;
  }

}
