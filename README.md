# Linux-System-Calls
This repository features code examples demonstrating key Unix/Linux system calls such as open(), read(), write(), fork(), dup(), dup2(), pipes, shared memory, and FIFOs. It also includes practical usage of various Linux commands. Ideal for those exploring process management, file handling, IPC mechanisms, and low-level system programming.


1-**Custom Shell - Command Line Interpreter**
This project implements a simple command-line interpreter in C, designed to execute common Linux commands. The shell supports:
Command execution with tokenization (e.g., ls, cp, cd, pwd, mv, mkdir, rmdir, rm, touch, grep).
Continuous user interaction with child processes created using system calls like execvp or execlp.
The program runs until the user types exit.


**2-Automated Production Line Process Management**
This C program simulates process management for a company's automated production line, consisting of four stages: Material Preparation, Assembly, Quality Check, and Packaging.

**Key Features**:
Stage Execution: Each stage runs in a separate process and simulates work by sleeping for a few seconds.
Process Coordination: Ensures each stage completes before the next starts using waitpid(). In case of failure, a stage is retried up to two times.
Error Handling: Logs issues and terminates the line if a stage fails after multiple attempts.
Completion Report: Provides a summary of successful completions and retry attempts.
The program makes use of process management techniques like fork(), waitpid(), exit(), and signal handling, while simulating random failures with a 30% chance per stage.

**3-Secure Data Transformation**
This C/C++ program securely transforms critical data from the compromised file classified_data.txt. It allows for safe analysis by either decrypting the data or redacting sensitive information.

**Key Features:**
Decryption: Uses a simple Caesar cipher to shift each character backward by one (if a decryption key is available).
Redaction: Replaces sensitive information like SSNs (e.g., "123-45-6789") with [REDACTED].
Parent-Child Process Interaction: The transformation is securely handled by a child process, ensuring sensitive data remains protected.
File I/O Handling: The parent process manages file writing and generates a summary report.
Decryption Example: Input "Uifsf jt b tfdsfu npofz" becomes "There is a secret money".
Redaction Example: Input "SSN: 123-45-6789" becomes "SSN: [REDACTED]".



