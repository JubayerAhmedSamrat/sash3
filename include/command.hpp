#pragma once
#include <string>
#include <vector>

struct Command
{
  std::vector<std::string> argv;

  std::string input_file;
  std::string output_file;

  bool append = false;
};
