#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define STAGES 4
#define MAX_ATTEMPTS 3

void simulate_stage(int stage) {
    if (rand() % 10 < 3) { // 30% chance of failure
        exit(1); // Simulate failure
    }
    sleep(2); // Simulate work
    exit(0); // Success
}

int main() {
    srand(time(NULL));
    int attempts[STAGES] = {0};
    const char *stage_names[] = {"Material Preparation", "Assembly", "Quality Check", "Packaging"};

    for (int i = 0; i < STAGES; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            simulate_stage(i);
        } else if (pid > 0) {
            int status;
            while (attempts[i] < MAX_ATTEMPTS) {
                waitpid(pid, &status, 0);
                if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                    printf("%s completed successfully.\n", stage_names[i]);
                    break;
                } else {
                    attempts[i]++;
                    printf("%s failed. Attempt %d of %d.\n", stage_names[i], attempts[i], MAX_ATTEMPTS);
                    if (attempts[i] < MAX_ATTEMPTS) {
                        pid = fork();
                        if (pid == 0) {
                            simulate_stage(i);
                        }
                    } else {
                        printf("Error: %s failed after %d attempts. Terminating production line.\n", stage_names[i], MAX_ATTEMPTS);
                        exit(1);
                    }
                }
            }
        } else {
            perror("Fork failed");
            exit(1);
        }
    }

    printf("Production line completed successfully.\n");
    return 0;
}
