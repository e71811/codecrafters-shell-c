# Build Your Own Shell (C) - Work In Progress

This project is a POSIX-compliant shell implementation built from scratch in C. The primary goal is to gain a deep understanding of low-level process management, file system interactions, and complex string parsing.

## Development Status
The project is currently under active development. The current version focuses on the core execution engine and advanced argument parsing.

## Key Technical Milestones

* **Advanced Custom Tokenization (BetterStrTok):** Developed a custom parser to handle single quotes ('), double quotes ("), and backslash escaping (\), replacing the limited standard strtok function.
* **Process Management:** Implemented robust program execution using fork() and execve() to ensure child processes run in isolation from the main shell.
* **Built-in Command Suite:** Native implementation of exit, echo, pwd, type, and cd (including HOME directory ~ support).
* **Dynamic PATH Resolution:** Implemented a lookup mechanism to locate and execute system binaries across the environment PATH.

## Tech Stack
* **Language:** C (POSIX API)
* **Build System:** CMake
* **Operating System:** Linux/WSL

## Roadmap

### Core Features
- [x] Basic REPL loop and command recognition
- [x] External command execution via PATH lookup
- [x] Advanced argument parsing (Quotes and Escaping)

### Upcoming Milestones
- [ ] I/O Redirection: Support for redirecting and appending stdout/stderr.
- [ ] Pipelines: Implementation of dual-command and multi-command pipelines.
- [ ] Command & Filename Completion: Tab-completion for built-ins, executables, and system paths.
- [ ] Command History: Built-in history tracking with up/down arrow navigation.
- [ ] History Persistence: Ability to read and write command history to files on startup and exit.
- [ ] Signal Handling: Proper management of job control signals like Ctrl+C.
