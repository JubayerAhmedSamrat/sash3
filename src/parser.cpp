#include "parser.hpp"

Command Parser::parse(const std::vector<std::string>& tokens)
{
  Command command;

  for(std::size_t i = 0; i < tokens.size(); ++i)
  {
    if(tokens[i] == ">")
    {
      if(i + 1 < tokens.size())
      {
        command.output_file = tokens[i + 1];
      }
      break;
    }
    
    if(tokens[i] == ">>")
    {
      if(i + 1 < tokens.size())
      {
        command.output_file = tokens[i + 1];
        command.append = true;
      }
      break;
    }

    if(tokens[i] == "<")
    {
      if(i + 1 < tokens.size())
      {
        command.input_file = tokens[i + 1];
      }
      break;
    }
    command.argv.push_back(tokens[i]);
  }
  return command;
}
