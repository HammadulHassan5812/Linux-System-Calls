#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

void Execute(char *input)
{
    char *command[1024];
    char *token;
    int Index = 0;
    token = strtok(input, " \n");
    while (token != NULL)
    {
        command[Index++] = token;
        token = strtok(NULL, " \n");
    }
    command[Index] = NULL;
    if (strcmp(command[0], "exit") == 0)
    {
        exit(0);
    }

    pid_t pid = fork();
    if (pid == 0)
    {
        int execution = execvp(command[0], command);
        // if(execution)
        printf("Execution Status %d", execution);
        if (execution == -1)
        {
            perror("Execution Failed");
            exit(EXIT_FAILURE);
        }
    }
    else if (pid)
    {

        wait(NULL);
    }
    if (pid == -1)
    {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
}
void handleMain()
{
    char input[1024];

    printf("Starting Custom Shell..... \n");
    while (true)
    {
        printf("myCustomShell> ");
        char *inputToken = fgets(input, sizeof(input), stdin);
        if (inputToken == NULL)
        {
            printf("Null Input Detected! ");
            break;
        }
        Execute(input);
    }
}

int main()
{
    handleMain();
    return 0;
}
