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
        command.output_file = tokens[++i];
      }
    }else if(tokens[i] == ">>")
    {
      if(i + 1 < tokens.size())
      {
        command.output_file = tokens[++i];
        command.append = true;
      }
    }else if(tokens[i] == "<")
    {
      if(i + 1 < tokens.size())
      {
        command.input_file = tokens[++i];
      }
    } else {
      
      command.argv.push_back(tokens[i]);
    }
  }
  return command;
}
