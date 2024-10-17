#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO_REQ "/tmp/fifo_request"
#define FIFO_RES "/tmp/fifo_response"

int evaluate_expression(char *expression) {
    int num1, num2;
    char operator;
    sscanf(expression, "%d %c %d", &num1, &operator, &num2);
    
    switch (operator) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        case '/': return num1 / num2;
        default: return 0;
    }
}

int main() {
    char expression[256];
    char result[256];
    int fd_req, fd_res;

    // Create the FIFO pipes if they don't exist
    mkfifo(FIFO_REQ, 0666);
    mkfifo(FIFO_RES, 0666);

    while (1) {
        // Read the expression from the client
        fd_req = open(FIFO_REQ, O_RDONLY);
        if (fd_req < 0) {
            perror("Error opening request FIFO");
            exit(EXIT_FAILURE);
        }
        read(fd_req, expression, sizeof(expression));
        close(fd_req);

        // Evaluate the expression
        int res = evaluate_expression(expression);
        snprintf(result, sizeof(result), "%d", res);

        // Send the result back to the client
        fd_res = open(FIFO_RES, O_WRONLY);
        if (fd_res < 0) {
            perror("Error opening response FIFO");
            exit(EXIT_FAILURE);
        }
        write(fd_res, result, strlen(result) + 1);
        close(fd_res);
    }

    return 0;
}
