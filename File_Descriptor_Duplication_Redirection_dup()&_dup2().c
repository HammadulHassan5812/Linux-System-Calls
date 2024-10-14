#include <iostream>
#include <fcntl.h>     // For open()
#include <unistd.h>    // For dup(), dup2(), read() and close()
#include <sys/types.h> // For open() and read()
#include <sys/stat.h>  // For open()
#include <cstring>     // For strtok()
#include <cstdio>      // For perror()

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file> <error_file>\n";
        return 1;
    }

    // Open the input file
    int input_fd = open(argv[1], O_RDONLY);
    if (input_fd == -1) {
        perror("Error opening input file");
        return 1;
    }

    // Open the output and error files
    int output_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd == -1) {
        perror("Error opening output file");
        close(input_fd);
        return 1;
    }

    int error_fd = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (error_fd == -1) {
        perror("Error opening error file");
        close(input_fd);
        close(output_fd);
        return 1;
    }

    // Save the original stdout and stderr
    int saved_stdout = dup(STDOUT_FILENO);
    int saved_stderr = dup(STDERR_FILENO);

    if (saved_stdout == -1 || saved_stderr == -1) {
        perror("Error saving file descriptors");
        close(input_fd);
        close(output_fd);
        close(error_fd);
        return 1;
    }

    // Redirect stdout to output file and stderr to error file
    if (dup2(output_fd, STDOUT_FILENO) == -1) {
        perror("Error redirecting stdout");
        return 1;
    }
    if (dup2(error_fd, STDERR_FILENO) == -1) {
        perror("Error redirecting stderr");
        return 1;
    }

    // Process the input file
    cout << "Processing input file...\n";  // This goes to output.txt

    // Buffer to hold file content
    char buffer[100];
    ssize_t bytesRead = read(input_fd, buffer, sizeof(buffer) - 1);

    if (bytesRead == -1) {
        perror("Error reading input file");
        return 1;
    }

    buffer[bytesRead] = '\0';  // Null-terminate buffer

    // Tokenize the buffer
    char* token = strtok(buffer, " \n");

    while (token != nullptr) {
        bool isNumber = true;
        for (int i = 0; i < strlen(token); i++) {
            if (!isdigit(token[i])) {
                isNumber = false;
                break;
            }
        }

        if (isNumber) {
            int number = atoi(token);
            if (number == 0) {
                cerr << "Error: Division by zero encountered.\n";  // Goes to error.txt
            } else {
               // cout << "Read number: " << number << endl;           // Goes to output.txt
                cout << "Processed number: " << number * 2 << endl;  // Processing example: multiply by 2
            }
        } else {
            cerr << "Error: Invalid number format for '" << token << "'\n";  // Goes to error.txt
        }

        // Move to the next token
        token = strtok(nullptr, " \n");
    }

    // Restore original stdout and stderr
    if (dup2(saved_stdout, STDOUT_FILENO) == -1 || dup2(saved_stderr, STDERR_FILENO) == -1) {
        perror("Error restoring file descriptors");
        return 1;
    }

    // Close the files
    close(input_fd);
    close(output_fd);
    close(error_fd);

    // Output to console confirming restoration
    cout << "File descriptors restored to original states.\n";

    return 0;
}
