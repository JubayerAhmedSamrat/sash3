**ARCHITECTURE: The shape of the system.**

# INDEX

    1. Purpose
    2. Design Principles
    3. Directory Structure
    4. Modules and Responsibility
    5. Execution Flow
    6. Future Architecture

## Purpose
`sash3` is a Unix-like shell implemented in Modern C++23.

The project exists primarily for:
- Learning system programming.
- Understanding Linux process management.
- Understanding shell internals.
- Practicing modern C++ design.
- Building a production-style C++ project from scratch.

The shell will gradually support:

- Command execution.
- Built-in commands.
- Pipes. 
- Redirections.
- Job control.
- Signal handling.
- Command history.
## Design Principles of sash3
The Architecture of sash3 follows 3 major principles.

    1. Layered Architecture
    2. Modular Monolith
    3. Separation of Concern(SoC)
### Layered Architecture
```text

    User Input -> Shell loop -> Lexer -> Parser -> Executor -> Linux System Calls

```
Each Layer has a signle Responsibility.
### Modular Monolith

The entire project is built as one executable: `sash3.exe`
Internally, it is divided into dependendent modules:
```text

    1. Shell
    2. Lexer
    3. Parser
    4. Executor
    5. Pipeline
    6. Jobs
    7. Signals
    8. Builtins
    9. Utils
```
### Separation of concern (SoC)
Each module should do **one thing** and do it **well**.
Responsibilities are separated to reduce coupling and improve maintainability.
## Directory Structure
```text

sash3/
|
|--- CMakeLists.txt
|--- include/
|--- src/
|--- docs/
|--- scripts/
|--- build/

```
### include/
Contains public declarations.
### src/
Contains implementations.
### tests/
Contains unit and integration tests.
### docs/
Conatains porject documentation.
### scripts/
Conatains helper scripts.
### build/
Conatains generated build artifacts.
## Modules and Responsibility
|Module     | Responsibility |
------------|-----------------
| shell | REPL(Read Evaluate print loop) loop and orchestration |
| Parser | Build command structures |
| Executor | Execute commands |
| Pipeline | Handle pipes |
| Jobs | Background job control |
| Signals | Signal handling |
| Builtins | shell built-in commands |
| Utils | Shared helper functions |


## Execution Flow
A command travels through the system as follows:

```text

user types command
shell reads Input
Lexer generateds tokens
Parser builds command representation
Executor executes commands
kernel performs system Calls
shell waits for next command
```
Example:
```text
ls -la | grep cpp

```
will eventually become:
```text
Input
tokens
parsed commands
process + pipes 
output
```
## Future Architecture
Planned features:
- Environment variable expansion.
- Command history.
- Auto-completion.
- Configuration files.
- Aliases.
- Scripting support.
- Better error handling.
- More comprehensive test.
