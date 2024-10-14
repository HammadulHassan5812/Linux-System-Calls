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


**4-BattleShip Game**
This is a C++ implementation of the classic Battleship game, where two players (Player 1 and Player 2) play on separate 5x5 grids to guess and sink each other's ships. The game uses pipes for communication between two processes representing the players, ensuring turn-based gameplay with real-time interaction.

**Game Rules:**
Each player has a 5x5 grid, with 3 ships placed at the beginning of the game.

**Ship Sizes:**
Ship 1: 2 spaces
Ship 2: 3 spaces
Ship 3: 4 spaces
Ships can be placed horizontally or vertically on the grid, and overlapping ships are not allowed.
Players take turns to guess the location of their opponent's ships by providing row and column coordinates.
Each guess results in either a Hit (H) or Miss (M).
The goal is to sink all the opponent's ships by guessing their locations.
The game ends when one player successfully sinks all three ships of the opponent.

**Communication Between Players:**
The game uses two pipes for communication between Player 1 and Player 2:
Pipe 1: Player 1 sends their guesses to Player 2.
Pipe 2: Player 2 sends their guesses to Player 1.
After each guess, the player will wait for the response from the other player (either H for Hit or M for Miss), and then proceed to their next turn.

**Features:**
Turn-Based Gameplay: Each player must wait for the other player's move before taking their next turn.
Hit/Miss Feedback: Players are informed whether their guess hits or misses the opponent's ships.
Error Handling: Proper handling of communication failures through pipes ensures smooth gameplay.

**Game Flow:**
Player 1's Turn:
Place ships on the grid.
Send a guess (row, col) through Pipe 1.
Wait for a response (Hit/Miss).
If all Player 2's ships are hit, Player 1 wins and the game ends.
Player 2's Turn:
Place ships on the grid.
Receive Player 1's guess from Pipe 1.
Check for Hit/Miss and respond through Pipe 1.
Send their own guess (row, col) through Pipe 2.
Wait for Player 1's response (Hit/Miss).
If all Player 1's ships are hit, Player 2 wins and the game ends.

**Requirements:**
Pipes: For bidirectional communication between Player 1 and Player 2.
fork(): To create separate processes for Player 1 and Player 2.
waitpid(): To ensure synchronization between player moves and prevent multiple guesses by the same player in a row.
Error Handling: For managing errors in pipe communication, ensuring the game does not crash unexpectedly.
How to Compile and Run:

**Compile the game:**
g++ battleship.cpp -o battleship
Run the game:
./battleship




**5-File Descriptor Duplication and Redirection in C++**
This C++ program demonstrates file input/output redirection using file descriptor duplication and redirection techniques (dup() and dup2()). The program reads data from an input file, processes it, and redirects standard output and standard error to specified output files.

**Overview:**
Input File: Reads data from a specified input file.
Output File: Redirects standard output (stdout) to a specified output file.
Error File: Redirects standard error (stderr) to a specified error file.

**Functionality:**

**File Handling:**
The program accepts three command-line arguments:
Input File: The file to read data from (e.g., input.txt).
Output File: The file to redirect standard output to (e.g., output.txt).
Error File: The file to redirect standard error to (e.g., error.txt).
It opens the input file for reading and the output/error files for writing.

**Duplicating and Redirecting stdout and stderr:**
The original stdout and stderr file descriptors are saved using dup().
dup2() is used to redirect stdout to the output file and stderr to the error file.

**Processing Input:**
The program reads lines from the input file and processes the data:
For each valid number, it outputs a processed number (e.g., performing operations like multiplication).
It handles invalid numbers (e.g., non-numeric values) and division by zero, outputting error messages to stderr (which is redirected).

**Restoring Original File Descriptors:**
After processing, the original stdout and stderr are restored using dup() and dup2().
A confirmation message is printed to the console indicating that the file descriptors have been restored.

**Error Handling:**
The program handles invalid file paths for the input, output, and error files.
If the input file cannot be opened, an appropriate error message is output using stderr (without redirection).



**6-Complex Number Array Processing in C**
This C program demonstrates the use of **shared memory** to perform various operations on an array of complex numbers using multiple child processes. The program computes the complex conjugate, magnitude, and sum of the provided complex numbers.


**7-Multi-Stage File Processing in C**
This C program implements a multi-stage file processing system that utilizes named pipes (FIFOs) for inter-process communication. The program reads an array of integers from a file and performs multiple transformations across different stages.


**Compile the program:**
gcc multi_stage_processing.c -o multi_stage_processing


**Run the program with command-line arguments:**
./multi_stage_processing input.txt 20








