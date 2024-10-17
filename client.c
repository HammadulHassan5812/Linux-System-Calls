#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO_REQ "/tmp/fifo_request"
#define FIFO_RES "/tmp/fifo_response"

int main() {
    char expression[256];
    char result[256];
    int fd_req, fd_res;

    // Create the FIFO pipes if they don't exist
    mkfifo(FIFO_REQ, 0666);
    mkfifo(FIFO_RES, 0666);

    while (1) {
        printf("Enter a mathematical expression (e.g., 5 + 3): ");
        fgets(expression, sizeof(expression), stdin);

        // Send the expression to the server
        fd_req = open(FIFO_REQ, O_WRONLY);
        if (fd_req < 0) {
            perror("Error opening request FIFO");
            exit(EXIT_FAILURE);
        }
        write(fd_req, expression, strlen(expression) + 1);
        close(fd_req);

        // Read the result from the server
        fd_res = open(FIFO_RES, O_RDONLY);
        if (fd_res < 0) {
            perror("Error opening response FIFO");
            exit(EXIT_FAILURE);
        }
        read(fd_res, result, sizeof(result));
        printf("Result: %s\n", result);
        close(fd_res);
    }

    return 0;
}
