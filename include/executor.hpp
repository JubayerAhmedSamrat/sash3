#pragma once
#include "jobs.hpp"
#include "builtin.hpp"
#include "pipeline.hpp"
#include <vector>
#include <string>

class Executor
{
public:
  explicit Executor(JobsManager& jobs);

  int execute(const Pipeline& pipeline);
  
  void printJobs() const;
  void removeJob(pid_t pid);
  
  void foregroundLastJob();
  void backgroundLastJob();

private:
  int executeSingle(const Command& command, bool background);
  void setupRedirection(const Command& command);


  [[noreturn]]
  void execCommand(const Command& command);
 
  JobsManager& jobs_;
  BuiltinManager builtin_;
};
