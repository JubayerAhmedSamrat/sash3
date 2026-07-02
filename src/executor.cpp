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

  int previous_read = - 1;
  std::vector<pid_t> children;

  for(size_t i = 0; i < pipeline.commands.size(); ++i)
  {
    int pipefd[2];

    bool last = (i == pipeline.commands.size() - 1);

    if(!last)
    {
      if(pipe(pipefd) < 0)
      {
        perror("pipe");
        return 1;
      }
    }

    pid_t pid = fork();

    if(pid < 0)
    {
      perror("fork");
      return 1;
    }

    if(pid == 0)
    {
      //read from previous pipe

      if(previous_read != -1)
      {
        dup2(previous_read, STDIN_FILENO);
      }

      //write to next pipe 
      if(!last)
      {
        dup2(pipefd[1], STDOUT_FILENO);
      }

      if(previous_read != -1)
      {
        close(previous_read);
      }

      if(!last)
      {
        close(pipefd[0]);
        close(pipefd[1]);
      }

      setupRedirection(pipeline.commands[i]);
      execCommand(pipeline.commands[i]);
    }

    children.push_back(pid);

    if(previous_read != -1)
    {
      close(previous_read);
    }

    if(!last)
    {
      close(pipefd[1]);
      previous_read = pipefd[0];
    }
  }

  int status = 0;

  for(pid_t pid : children)
  {
    waitpid(pid, &status, 0);
  }

  if(WIFEXITED(status))
  {
    return WEXITSTATUS(status);
  }

  return 1;
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
    
    setupRedirection(command);

    execCommand(command);
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

[[noreturn]]
void Executor::execCommand(const Command& command)
{
  std::vector<char*> argv;

  for(const auto& token : command.argv)
  {
    argv.push_back(const_cast<char*>(token.c_str()));
  }
  
  argv.push_back(nullptr);

  execvp(argv[0], argv.data());

  perror("execvp");
  std::exit(EXIT_FAILURE);
}

void Executor::setupRedirection(const Command& command)
{
  if(!command.input_file.empty())
  {

    int fd = open(command.input_file.c_str(), O_RDONLY);

    if(fd < 0)
    {
      perror("open");
      std::exit(EXIT_FAILURE);
    }

    dup2(fd, STDIN_FILENO);
    close(fd);
  }

  if(!command.output_file.empty())
  {
    int flags = O_WRONLY | O_CREAT;

    if(command.append)
    {
      flags |= O_APPEND;
    } else {
      flags |= O_TRUNC;
    }

    int fd = open(command.output_file.c_str(), flags, 0644);

    if(fd < 0)
    {
      perror("open");
      std::exit(EXIT_FAILURE);
    }

    dup2(fd, STDOUT_FILENO);
    close(fd);
  }
}
