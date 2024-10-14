#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>
#include <complex.h>

void complex_conjugate(double *shared_real, double *shared_imag, double *conj_real, double *conj_imag, int N) {
    for (int i = 0; i < N; i++) {
        conj_real[i] = shared_real[i];
        conj_imag[i] = -shared_imag[i];  // Conjugate: invert the sign of the imaginary part
    }
}

void complex_magnitude(double *shared_real, double *shared_imag, double *magnitude, int N) {
    for (int i = 0; i < N; i++) {
        magnitude[i] = sqrt(shared_real[i] * shared_real[i] + shared_imag[i] * shared_imag[i]);  // Magnitude: sqrt(a^2 + b^2)
    }
}

void complex_sum(double *shared_real, double *shared_imag, double *sum_real, double *sum_imag, int N) {
    *sum_real = 0.0;
    *sum_imag = 0.0;
    for (int i = 0; i < N; i++) {
        *sum_real += shared_real[i];
        *sum_imag += shared_imag[i];  // Sum real and imaginary parts separately
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: ./program N real1 imag1 real2 imag2 ...\n");
        return 1;
    }

    int N = atoi(argv[1]);  // Size of array
    int i;

    // Check if correct number of arguments
    if (argc != 2 * N + 2) {
        printf("Invalid number of arguments.\n");
        return 1;
    }

    // Create shared memory segments for real and imaginary parts of the input array
    int shm_real_id = shmget(IPC_PRIVATE, N * sizeof(double), IPC_CREAT | 0666);
    int shm_imag_id = shmget(IPC_PRIVATE, N * sizeof(double), IPC_CREAT | 0666);

    // Attach shared memory for real and imaginary parts
    double *shared_real = (double *)shmat(shm_real_id, NULL, 0);
    double *shared_imag = (double *)shmat(shm_imag_id, NULL, 0);

    // Store input complex numbers in shared memory
    for (i = 0; i < N; i++) {
        shared_real[i] = atof(argv[2 + 2 * i]);
        shared_imag[i] = atof(argv[3 + 2 * i]);
    }

    // Create shared memory for results
    int shm_conj_real_id = shmget(IPC_PRIVATE, N * sizeof(double), IPC_CREAT | 0666);
    int shm_conj_imag_id = shmget(IPC_PRIVATE, N * sizeof(double), IPC_CREAT | 0666);
    int shm_magnitude_id = shmget(IPC_PRIVATE, N * sizeof(double), IPC_CREAT | 0666);
    int shm_sum_real_id = shmget(IPC_PRIVATE, sizeof(double), IPC_CREAT | 0666);
    int shm_sum_imag_id = shmget(IPC_PRIVATE, sizeof(double), IPC_CREAT | 0666);

    // Attach shared memory for results
    double *conj_real = (double *)shmat(shm_conj_real_id, NULL, 0);
    double *conj_imag = (double *)shmat(shm_conj_imag_id, NULL, 0);
    double *magnitude = (double *)shmat(shm_magnitude_id, NULL, 0);
    double *sum_real = (double *)shmat(shm_sum_real_id, NULL, 0);
    double *sum_imag = (double *)shmat(shm_sum_imag_id, NULL, 0);

    // Fork child processes
    pid_t pid1 = fork();
    if (pid1 == 0) {
        // Child 1: Compute complex conjugates
        complex_conjugate(shared_real, shared_imag, conj_real, conj_imag, N);
        exit(0);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        // Child 2: Compute magnitudes
        complex_magnitude(shared_real, shared_imag, magnitude, N);
        exit(0);
    }

    pid_t pid3 = fork();
    if (pid3 == 0) {
        // Child 3: Compute sum of complex numbers
        complex_sum(shared_real, shared_imag, sum_real, sum_imag, N);
        exit(0);
    }

    // Wait for all child processes to finish
    wait(NULL);
    wait(NULL);
    wait(NULL);

    // Parent process: print results
    printf("Complex Conjugates:\n");
    for (i = 0; i < N; i++) {
        printf("%lf + %lfi\n", conj_real[i], conj_imag[i]);
    }

    printf("\nMagnitudes:\n");
    for (i = 0; i < N; i++) {
        printf("%lf\n", magnitude[i]);
    }

    printf("\nSum of Complex Numbers:\n");
    printf("%lf + %lfi\n", *sum_real, *sum_imag);

    // Detach shared memory
    shmdt(shared_real);
    shmdt(shared_imag);
    shmdt(conj_real);
    shmdt(conj_imag);
    shmdt(magnitude);
    shmdt(sum_real);
    shmdt(sum_imag);

    // Destroy shared memory
    shmctl(shm_real_id, IPC_RMID, NULL);
    shmctl(shm_imag_id, IPC_RMID, NULL);
    shmctl(shm_conj_real_id, IPC_RMID, NULL);
    shmctl(shm_conj_imag_id, IPC_RMID, NULL);
    shmctl(shm_magnitude_id, IPC_RMID, NULL);
    shmctl(shm_sum_real_id, IPC_RMID, NULL);
    shmctl(shm_sum_imag_id, IPC_RMID, NULL);

    return 0;
}
