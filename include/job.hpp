#pragma once
#include <sys/types.h>
#include <string>

enum class JobState
{
  Running,
  Stopped
};

struct Job
{
  int id;
  pid_t pid;
  std::string command;
  JobState state = JobState::Running;
};
