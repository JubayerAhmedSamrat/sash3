#pragma once

#include "job.hpp"
#include <vector>
#include <string>
#include <sys/types.h>

class JobsManager
{
public:
  void add(pid_t pid,
      const std::string& command);

  void remove(pid_t pid);

  void print() const;
  void stop(pid_t pid);
  void resume(pid_t pid);

  std::vector<Job>& jobs();
  Job* lastJob();
  Job* findJob(int id);
private:

  std::vector<Job> jobs_;

  int next_job_id_ = 1;
};
