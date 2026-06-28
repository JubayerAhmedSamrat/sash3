#pragma once

#include <string>
#include <vector>

class Lexer 
{
public:
  std::vector<std::string>
    tokenize(const std::string& line);
};
