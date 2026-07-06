#include "shell.hpp"
#include <iostream>
#include <csignal>
#include <unistd.h>
#include <signal.h>
#include <termios.h>

void sigintHandler(int)
{
  std::cout <<'\n';
}

int main()
{
  // put the shell in its own process group
  setpgid(0, 0);
  // take control of the terminal
  tcsetpgrp(STDIN_FILENO, getpid());

  // Ignore job-control signals 
  signal(SIGINT, SIG_IGN); 
  signal(SIGTSTP, SIG_IGN);
  signal(SIGTTOU, SIG_IGN);
  signal(SIGTTIN, SIG_IGN);

  Shell shell;
  shell.run();

  return 0;

}
