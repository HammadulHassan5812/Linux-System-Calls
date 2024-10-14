#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 256
#define REDACTED "[REDACTED]"

void decrypt_data(FILE *input, FILE *output) {
    char ch;
    while ((ch = fgetc(input)) != EOF) {
        if (ch >= 'A' && ch <= 'Z') {
            // Decrypt uppercase letters, wrap 'A' to 'Z'
            fputc(ch == 'A' ? 'Z' : ch - 1, output);
        } else if (ch >= 'a' && ch <= 'z') {
            // Decrypt lowercase letters, wrap 'a' to 'z'
            fputc(ch == 'a' ? 'z' : ch - 1, output);
        } else {
            // Leave non-alphabet characters unchanged
            fputc(ch, output);
        }
    }
}


void redact_data(FILE *input, FILE *output) {
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, input) != NULL) {
        char *pos = strstr(buffer, "SSN: ");
        if (pos) {
            strncpy(pos + 5, REDACTED, strlen(REDACTED));
        }
        fputs(buffer, output);
    }
}

int main() {
    char input_file[100], output_file[100];
    int choice;

    printf("Enter input file name: ");
    scanf("%s", input_file);
    printf("Enter output file name: ");
    scanf("%s", output_file);
    printf("Choose transformation option (1: Decrypt, 2: Redact): ");
    scanf("%d", &choice);

    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }

    if (pid == 0) { // Child process
        FILE *input = fopen(input_file, "r");
        FILE *output = fopen(output_file, "w");
        if (!input || !output) {
            perror("File opening failed");
            exit(1);
        }

        if (choice == 1) {
            decrypt_data(input, output);
        } else if (choice == 2) {
            redact_data(input, output);
        } else {
            fprintf(stderr, "Invalid choice\n");
            exit(1);
        }

        fclose(input);
        fclose(output);
        exit(0);
    } else { // Parent process
        wait(NULL);
        printf("Transformation complete. Check the output file.\n");
    }

    return 0;
}
