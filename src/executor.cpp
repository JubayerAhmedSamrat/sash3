#include "executor.hpp"

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <csignal>

Executor::Executor(JobsManager& jobs) : jobs_(jobs)
{

}
int Executor::execute(
    const Pipeline& pipeline)
{

  if(pipeline.commands.empty())
  {
    return 0;
  }

  if(pipeline.commands.size() == 1)
  {
    return executeSingle(pipeline.commands[0],
        pipeline.background);
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
      signal(SIGINT, SIG_DFL);
      signal(SIGTSTP, SIG_DFL);
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

int 
Executor::executeSingle(const Command& command, bool background)
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

  setpgid(pid, pid);

  if(pid == 0)
  {
  //child porcess-----------------------------
    setpgid(0, 0);

    if(!background)
    {
      tcsetpgrp(STDIN_FILENO, getpid());
    }

    signal(SIGINT, SIG_DFL);
    signal(SIGTSTP, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);

    setupRedirection(command);

    execCommand(command);
  } else {
  //parent process-------------------------------
    int status;
   
    if(background)
    {
      std::string command_line;

      for(const auto& token : command.argv)
      {
        if(!command_line.empty())
        {
          command_line += " ";
        }

         command_line += token;
      }

      jobs_.add(pid, command_line);

      return 0;
    }
    waitpid(pid, &status, WUNTRACED);
   
    tcsetpgrp(STDIN_FILENO, getpid());

    if(WIFSTOPPED(status))
    {
      std::string command_line;

      for(const auto& token : command.argv)
      {
        if(!command_line.empty())
        {
          command_line += " ";
        }

        command_line += token;
      }

      jobs_.add(pid, command_line);
      jobs_.stop(pid);

      return 0;
    }
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

void Executor::printJobs() const 
{
  jobs_.print();
}

void Executor::removeJob(pid_t pid)
{
  jobs_.remove(pid);
}

void Executor::foregroundLastJob()
{
  Job* job = jobs_.lastJob();

  if(job == nullptr)
  {
    std::cout << "fg: no current job\n";
    return;
  }

  std::cout << job->command <<'\n';
  
  // give terminal to the job
  tcsetpgrp(STDIN_FILENO, job->pid);

  // continue the entire process group
  kill(-job->pid, SIGCONT);

  int status;
  waitpid(job->pid, &status, 0);

  // shell gets terminal back 
  tcsetpgrp(STDIN_FILENO, getpgrp());

  if(WIFSTOPPED(status))
  {
    jobs_.stop(job->pid);
  } else 
  {
    jobs_.remove(job->pid);
  }

}

void Executor::foregroundJob(int id)
{
  Job* job = jobs_.findJob(id);

  if(job == nullptr)
  {
    std::cout << "fg: no such job\n";
    return;
  }

  jobs_.resume(job->pid);

  std::cout << job->command <<'\n';

  kill(-job->pid, SIGCONT);

  int status;
  waitpid(job->pid, &status, WUNTRACED);

  tcsetpgrp(STDIN_FILENO, getpid());

  if(WIFSTOPPED(status))
  {
    jobs_.stop(job->pid);
  } else 
  {
    jobs_.remove(job->pid);
  }
}

void Executor::backgroundJob(int id)
{
  Job* job = jobs_.findJob(id);

  if(job == nullptr)
  {
    std::cout << "bg: no such job\n";
    return;
  }

  if(job->state == JobState::Running)
  {
    std::cout << "bg: job already running\n";
    return;
  }

  kill(-job->pid, SIGCONT);
  jobs_.resume(job->pid);

  std::cout 
    << "["
    << job->id 
    << "] Running "
    << job->command 
    << '\n';
}
void Executor::backgroundLastJob()
{
  Job* job = jobs_.lastJob();

  if(job == nullptr)
  {
    std::cout << "bg: no current job\n";
    return;
  }

  // Already running?
  if(job->state == JobState::Running)
  {
    std::cout << "bg: job already running\n";
    return;
  }

  // Resume the entire process group 
  kill(-job->pid, SIGCONT);

  jobs_.resume(job->pid);

  std::cout 
    << "["
    << job->id 
    << "] Running "
    << job->command 
    <<'\n';
}
