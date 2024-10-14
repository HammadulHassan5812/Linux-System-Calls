# Linux-System-Calls
This repository features code examples demonstrating key Unix/Linux system calls such as open(), read(), write(), fork(), dup(), dup2(), pipes, shared memory, and FIFOs. It also includes practical usage of various Linux commands. Ideal for those exploring process management, file handling, IPC mechanisms, and low-level system programming.

1-**Custom Shell - Command Line Interpreter**
This project implements a simple command-line interpreter in C, designed to execute common Linux commands. The shell supports:
Command execution with tokenization (e.g., ls, cp, cd, pwd, mv, mkdir, rmdir, rm, touch, grep).
Continuous user interaction with child processes created using system calls like execvp or execlp.
The program runs until the user types exit.

