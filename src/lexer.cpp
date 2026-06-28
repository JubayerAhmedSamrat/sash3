#include "lexer.hpp"
#include <sstream>

std::vector<std::string>
Lexer::tokenize(const std::string& line)
{
  std::vector<std::string> tokens;

  std::stringstream ss(line);
  std::string token;

  while(ss >> token)
  {
    tokens.push_back(token);
  }
  return tokens;
}
