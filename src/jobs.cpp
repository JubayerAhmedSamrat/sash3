#include "jobs.hpp"

#include <iostream>
#include <algorithm>

void JobsManager::add(pid_t pid, const std::string& command)
{
  Job job;

  job.id = next_job_id_++;
  job.pid = pid;
  job.command = command;

  jobs_.push_back(job);

  std::cout  
    <<"[" 
    << job.id
    << "] "
    << job.pid 
    << '\n';
}

void JobsManager::remove(pid_t pid) 
{
  jobs_.erase(
      std::remove_if(
        jobs_.begin(),
        jobs_.end(),
        [pid](const Job& job)
        {
        return job.pid == pid;
        }),
      jobs_.end());
}

void JobsManager::print() const
{
  for(const auto& job : jobs_)
  {
    std::cout 
      << "["
      << job.id 
      <<"] ";

    if(job.state == JobState::Running)
    {
      std::cout << "Running ";
    } else 
    {
      std::cout << "Stopped ";
    }

    std::cout 
      << job.command 
      << '\n';
  }
}

std::vector<Job>& JobsManager::jobs()
{
  return jobs_;
}

Job* JobsManager::lastJob()
{
  if(jobs_.empty())
  {
    return nullptr;
  }

  return &jobs_.back();
}

Job* JobsManager::findJob(int id)
{
  for(auto& job : jobs_)
  {
    if(job.id == id)
    {
      return &job;
    }
  }
  
  return nullptr;
}

void JobsManager::stop(pid_t pid)
{
  for(auto& job : jobs_)
  {
    if(job.pid == pid)
    {
      job.state = JobState::Stopped;
      return;
    }
  }
}
void JobsManager::resume(pid_t pid)
{
  for(auto& job : jobs_)
  {
    if(job.pid == pid)
    {
      job.state = JobState::Running;
      return;
    }
  }
}
