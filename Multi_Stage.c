#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

void square_numbers(const char *input_file, const char *fifo1) {
    FILE *file = fopen(input_file, "r");
    if (!file) {
        perror("File open error");
        exit(1);
    }

    int fd = open(fifo1, O_WRONLY);
    if (fd == -1) {
        perror("FIFO1 open error");
        exit(1);
    }

    int num;
    while (fscanf(file, "%d", &num) != EOF) {
        int squared = num * num;
        write(fd, &squared, sizeof(int));
    }

    close(fd);
    fclose(file);
}

void compute_running_avg(const char *fifo1, const char *fifo2) {
    int fd1 = open(fifo1, O_RDONLY);
    int fd2 = open(fifo2, O_WRONLY);
    if (fd1 == -1 || fd2 == -1) {
        perror("FIFO open error");
        exit(1);
    }

    int sum = 0, count = 0, num;
    while (read(fd1, &num, sizeof(int)) > 0) {
        sum += num;
        count++;
        float running_avg = (float)sum / count;
        write(fd2, &running_avg, sizeof(float));
    }

    close(fd1);
    close(fd2);
}

void count_above_threshold(const char *fifo2, const char *fifo3, float threshold) {
    int fd2 = open(fifo2, O_RDONLY);
    int fd3 = open(fifo3, O_WRONLY);
    if (fd2 == -1 || fd3 == -1) {
        perror("FIFO open error");
        exit(1);
    }

    float running_avg;
    int count = 0;
    while (read(fd2, &running_avg, sizeof(float)) > 0) {
        if (running_avg > threshold) {
            count++;
        }
    }

    write(fd3, &count, sizeof(int));

    close(fd2);
    close(fd3);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <input_file> <threshold>\n", argv[0]);
        exit(1);
    }

    const char *input_file = argv[1];
    float threshold = atof(argv[2]);

  
    const char *fifo1 = "fifo1";
    const char *fifo2 = "fifo2";
    const char *fifo3 = "fifo3";
    mkfifo(fifo1, 0666);
    mkfifo(fifo2, 0666);
    mkfifo(fifo3, 0666);

    pid_t pid1, pid2, pid3;

    
    if ((pid1 = fork()) == 0) {
        square_numbers(input_file, fifo1);
        exit(0);
    }

    
    if ((pid2 = fork()) == 0) {
        compute_running_avg(fifo1, fifo2);
        exit(0);
    }

    
    if ((pid3 = fork()) == 0) {
        count_above_threshold(fifo2, fifo3, threshold);
        exit(0);
    }

    
    int fd3 = open(fifo3, O_RDONLY);
    if (fd3 == -1) {
        perror("FIFO3 open error");
        exit(1);
    }

    int final_count;
    read(fd3, &final_count, sizeof(int));
    printf("%d running averages are greater than %f.\n", final_count, threshold);

    close(fd3);

  
    unlink(fifo1);
    unlink(fifo2);
    unlink(fifo3);

    return 0;
}
