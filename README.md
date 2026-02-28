# Build Your Own Shell (C) - Project Completed 🚀

This project is a fully functional, POSIX-compliant shell implementation built from scratch in C. It demonstrates advanced proficiency in low-level process management, filesystem navigation, and complex string parsing.

## 🛠 Key Technical Milestones

- **Advanced Custom Tokenization (BetterStrTok)**: A custom-built recursive parser that handles complex command-line inputs, including single/double quotes and backslash escaping.

- **Persistent Command History Engine**: Professional-grade integration with GNU Readline and the filesystem.
  * Supports environment-based configuration via `HISTFILE`.
  * Selective history synchronization using `history -a` (append only new entries).
  * Built-in history management with `-w` (write) and `-r` (read) flags.
  * Supports limiting displayed entries using `history <n>`.

- **Intelligent Context-Aware Autocompletion**: A comprehensive multi-tier Tab-completion engine.
  * **Built-in & PATH Recognition**: Quickly completes shell built-ins and global binaries.
  * **Nested Path Completion**: Supports deep filesystem navigation (e.g., `cat path/to/f<TAB>`).
  * **Directory Intelligence**: Automatically identifies directories and appends a trailing `/` (instead of a space) to facilitate rapid sub-folder traversal.

- **Modular I/O Redirection & Pipelines**:
  * Full support for stdout/stderr redirection and appending (`>`, `>>`, `2>`, `2>>`).
  * **Process Chaining**: Implemented dual-command pipelines using `pipe()` and `dup2()` for efficient inter-process communication.

## 📋 Features Overview

- [x] **REPL Loop**: Robust command recognition and processing loop.
- [x] **External Commands**: Full execution support via PATH lookup.
- [x] **Advanced Parsing**: Precise handling of quotes, spaces, and escaping.
- [x] **I/O Redirection**: Comprehensive management of file descriptors for redirection.
- [x] **Persistent History**: Reliable synchronization between shell sessions.
- [x] **Advanced Autocompletion**: Smart, directory-aware completion for files and commands.
- [x] **Process Pipelines**: Seamless chaining of commands using Unix pipes.