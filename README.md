# Build Your Own Shell (C) - Work In Progress

This project is a POSIX-compliant shell implementation built from scratch in C. The primary goal is to gain a deep understanding of low-level process management, file system interactions, and complex string parsing.

## Key Technical Milestones

- **Advanced Custom Tokenization (BetterStrTok)**: A custom-built recursive parser that handles single/double quotes and backslash escaping for complex command inputs.

- **Persistent Command History Engine**: Integrated GNU Readline with a custom persistence layer.
  * Supports environment-based configuration via `HISTFILE`.
  * Implements selective history synchronization with `history -a` (append only new entries).
  * Supports built-in history management with `-w` (write) and `-r` (read) flags.
  * Supports limiting displayed entries with `history <n>`.

- **Intelligent Tab-Completion**: Implemented a two-tier completion generator.
  * **Tier 1**: Built-in command recognition.
  * **Tier 2**: Dynamic `PATH` binary lookup using `opendir` and `readdir`.

- **Modular I/O Redirection & Pipelines**:
  * Support for stdout/stderr redirection (`>`, `>>`, `2>`, `2>>`).
  * **Dual-Command Pipelines**: Implemented inter-process communication using `pipe()` and `dup2()` to chain commands.

## Roadmap

### Core Features
- [x] Basic REPL loop and command recognition
- [x] External command execution via PATH lookup
- [x] Advanced argument parsing (Quotes and Escaping)
- [x] I/O Redirection: Full support for redirecting and appending stdout/stderr
- [x] Persistent History: Full suite of history management tools
- [x] Autocompletion: Command and Built-in completion using Readline
- [x] Pipelines: Implementation of dual-command pipelines (e.g., `cmd1 | cmd2`)

### Upcoming Milestones
- [ ] Signal Handling: Graceful handling of `Ctrl+C` and `Ctrl+Z`
- [ ] Job Control: Background process management (`&`, `fg`, `bg`)