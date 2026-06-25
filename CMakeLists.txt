cmake_minimum_required(VERSION 3.22)

project(sash3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

add_executable(sash3
  src/main.cpp
  src/shell.cpp
  src/parser.cpp
  src/lexer.cpp
  src/executor.cpp
  src/pipeline.cpp
  src/jobs.cpp
  src/signals.cpp
  src/builtin.cpp
  src/utils.cpp


)
