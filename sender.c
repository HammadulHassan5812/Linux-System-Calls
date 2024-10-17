 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO_NAME "/tmp/chat_fifo"

int main() {
    char message[256];
    int fd;

    // Create the FIFO if it doesn't exist
    mkfifo(FIFO_NAME, 0666);

    printf("Sender started. Type your messages:\n");

    while (1) {
        // Open the FIFO for writing
        fd = open(FIFO_NAME, O_WRONLY);
        if (fd < 0) {
            perror("Failed to open FIFO for writing");
            exit(EXIT_FAILURE);
        }

        // Read user input
        fgets(message, sizeof(message), stdin);

        // Write the message to the FIFO
        write(fd, message, strlen(message) + 1);

        // Close the FIFO
        close(fd);
    }

    return 0;
}

