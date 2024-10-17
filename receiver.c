 #include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO_NAME "/tmp/chat_fifo"

int main() {
    char message[256];
    int fd;

    // Create the FIFO if it doesn't exist
    mkfifo(FIFO_NAME, 0666);

    printf("Receiver started. Waiting for messages...\n");

    while (1) {
        // Open the FIFO for reading
        fd = open(FIFO_NAME, O_RDONLY);
        if (fd < 0) {
            perror("Failed to open FIFO for reading");
            exit(EXIT_FAILURE);
        }

        // Read the message from the FIFO
        if (read(fd, message, sizeof(message)) > 0) {
            printf("Received: %s", message);
        }

        // Close the FIFO
        close(fd);
    }

    return 0;
}
