#include "executor.hpp"
 
#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

int Executor::execute(
    const Pipeline& pipeline)
{

  if(pipeline.commands.empty())
  {
    return 0;
  }

  if(pipeline.commands.size() == 1)
  {
    return executeSingle(pipeline.commands[0]);
  }
  if(pipeline.commands.size() != 2)
  {
    return 1;
  }

  const Command& left = pipeline.commands[0];
  const Command& right = pipeline.commands[1];

  int pipefd[2];

  if(pipe(pipefd) < 0)
  {
    perror("pipe");
    return 1;
  }

  pid_t left_pid = fork();

  if(left_pid < 0)
  {
    perror("fork");
    close(pipefd[0]);
    close(pipefd[1]);
    return 1;
  }

  if(left_pid == 0)
  {
    //child 1 (left command)

    dup2(pipefd[1], STDOUT_FILENO);

    close(pipefd[0]);
    close(pipefd[1]);

    std::vector<char*> argv;

    for(const auto& token : left.argv)
    {
      argv.push_back(const_cast<char*>(token.c_str()));
    }
    argv.push_back(nullptr);

    execvp(argv[0], argv.data());

    perror("execvp");
    std::exit(EXIT_FAILURE);
  }

  pid_t right_pid = fork();

  if(right_pid < 0)
  {
    perror("fork");
    close(pipefd[0]);
    close(pipefd[1]);
    return 1;
  }

  if(right_pid == 0)
  {
    //child 2 (right command)
    
    dup2(pipefd[0], STDIN_FILENO);

    close(pipefd[0]);
    close(pipefd[1]);

    std::vector<char*> argv;

    for(const auto& token : right.argv)
    {
      argv.push_back(const_cast<char*>(token.c_str()));
    }

    argv.push_back(nullptr);

    execvp(argv[0], argv.data());

    perror("execvp");
    std::exit(EXIT_FAILURE);
  }

  // parent
  close(pipefd[0]);
  close(pipefd[1]);

  int status;

  waitpid(left_pid, &status, 0);
  waitpid(right_pid, &status, 0);
  
  return 0;
}

int Executor::executeSingle(const Command& command)
{
  
  const auto& tokens = command.argv;

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
  //child porcess-----------------------------
    if(!command.input_file.empty())
    {
      int fd = open(command.input_file.c_str(), O_RDONLY);
      if(fd < 0)
      {
        perror("open");
        std::exit(EXIT_FAILURE);
      }
      if(dup2(fd, STDIN_FILENO) < 0)
      {
        perror("dup2");
        close(fd);
        std::exit(EXIT_FAILURE);
      }
      close(fd);
    }
  // redirect stdout if needed-----
    if(!command.output_file.empty())
    {
      int flags = O_WRONLY | O_CREAT;
      if(command.append)
      {
        flags |= O_APPEND;
      } else {
        flags |= O_TRUNC;
      }
      int fd = open(
          command.output_file.c_str(),
          flags, 0644
          );
      if(fd < 0)
      {
        perror("open");
        std::exit(EXIT_FAILURE);
      }

      if(dup2(fd, STDOUT_FILENO) < 0)
      {
        perror("dup2");
        close(fd);
        std::exit(EXIT_FAILURE);
      }
      close(fd);
    }
    // build argv--------------
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
  //parent process-------------------------------
    int status;
    waitpid(pid, &status, 0);

    if(WIFEXITED(status))
    {
      return WEXITSTATUS(status);
    }
    
    return 1;
  }

}
