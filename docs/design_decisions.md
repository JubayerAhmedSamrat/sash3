**DESIGN DECISION: Why did i choose this shape?**
# INDEX

    DD-001: Modern c++23 
    DD-002: CMake
    DD-003: Layered Architecture
    DD-004: Modular Monolith
    DD-005: One Module per responsibility(SoC)

## DD-001: Modern c++23
### Status
    - Accepted.
### Decision 
    - Implement the shell using Modern c++23.
### Reason
    - Learn modern c++.
    - use safer and expressive language features.
    - Prepare for modern c++ system programming.
### Alternative
    - C++17
    - C
### Consequences
    - Requires a newer compiler.
    - Access to modern language features.

## DD-002: CMake
### Status
    - Accepted.
### Decision 
    - use CMake as the Build System.
### Reason
    - Cross-Platform.
    - industry standard.
    - Good IDE support.
### Alternatives 
    - Raw Makefiles
    - Premake
### Consequences
    - Need to learn CMake.
    - Easier project growth.

## DD-003: Layered Architecture
### Status
    - Accepted
### Descision
    - Separate the shell into layers:
    shell-> Lexer -> parser -> Executor -> OS.
### Reason
    - Easier to understand.
    - Easier to test.
    - Easier to modify.
### Alternative
    - Single large file.
    - Highly coupled design.
### Consequences
    - More files.
    - Better maintainability.
## DD-004: Modular Monolith
### Status
    - Accepted
### Descision
    - Build one executable with multiple internal modules.
### Reason
    - Simple to deployment.
    - Easy debugging.
    - Suitable for learning project.
### Alternative
    - Multiple libraries.
    - Plugin architecture.
### Consequences
    - Tighter coupling than microservices.
    - Simpler development.
## DD-005: One Module per responsibility(SoC)
### Status
    - Accepted
### Descision
    - Each module should have one primary responsibility.
### Reason
    - Easier testing.
    - Easier maintanance.
    - Better code organization.
### Alternative
    - Mixed responsibility.
### Consequences
    - More files.
    - Cleaner codebase.
