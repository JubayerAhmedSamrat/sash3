#include "parser.hpp"

Pipeline Parser::parse(const std::vector<std::string>& tokens)
{
  Pipeline pipeline;
  Command command;

  for(std::size_t i = 0; i < tokens.size(); ++i)
  {
    if(tokens[i] == "|")
    {
      pipeline.commands.push_back(command);
      command = Command{};
    }
    else if(tokens[i] == ">")
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

  if(!command.argv.empty())
  {
    pipeline.commands.push_back(command);
  }
  return pipeline;
}
