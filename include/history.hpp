#pragma once 

#include <string>
#include <vector>

class History 
{
public:
  void add(const std::string& command);

  void print() const;
private:
  std::vector<std::string> commands_;
};
