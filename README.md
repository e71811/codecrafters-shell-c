# Build Your Own Shell (C) - Work In Progress

This project is a POSIX-compliant shell implementation built from scratch in C. The primary goal is to gain a deep understanding of low-level process management, file system interactions, and complex string parsing.

## Key Technical Milestones

* **Advanced Custom Tokenization (`BetterStrTok`)**: Developed a custom parser to handle single quotes, double quotes, and backslash escaping.
* **Process Management**: Implemented robust program execution using `fork()` and `execv()` to ensure child processes run in isolation from the main shell.
* **Modular I/O Redirection Engine**: Implemented a centralized redirection system (`apply_redirection`) that manages file descriptors using `dup2()` and bitwise flags like `O_WRONLY | O_CREAT`.
    * Supports Output Redirection (`>`) and Append Mode (`>>`).
    * Supports Stderr Redirection (`2>`) and Stderr Append (`2>>`).

## Roadmap

### Core Features
- [x] Basic REPL loop and command recognition
- [x] External command execution via PATH lookup
- [x] Advanced argument parsing (Quotes and Escaping)
- [x] **I/O Redirection**: Support for redirecting and appending stdout/stderr

### Upcoming Milestones
- [ ] Pipelines: Implementation of dual-command and multi-command pipelines
- [x] Command and Filename Completion
- [ ] Command History tracking
- [ ] Signal Handling (Ctrl+C)