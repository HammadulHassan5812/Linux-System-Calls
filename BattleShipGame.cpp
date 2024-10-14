#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <cstdlib> 
#include <ctime>   

using namespace std;

const int SIZE = 5;

bool isOccupied(int row, int col, int arr[SIZE][SIZE]) {
    return arr[row][col] != 0;
}

void output(int player[SIZE][SIZE]) {
    cout << "Player's grid:\n";
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cout << player[i][j] << " ";
        }
        cout << endl;
    }
}

bool placeShip(int player[SIZE][SIZE], int shipSize) {
    int direction = rand() % 2; // 0 for horizontal, 1 for vertical
    int row, col;
    bool canPlace = false;

    while (!canPlace) {
        row = rand() % SIZE;
        col = rand() % SIZE;

        if (direction == 0 && col + shipSize <= SIZE) { // Horizontal placement
            canPlace = true;
            for (int i = 0; i < shipSize; i++) {
                if (isOccupied(row, col + i, player)) {
                    canPlace = false;
                    break;
                }
            }
            if (canPlace) {
                for (int i = 0; i < shipSize; i++) {
                    player[row][col + i] = shipSize;
                }
            }
        } else if (direction == 1 && row + shipSize <= SIZE) { // Vertical placement
            canPlace = true;
            for (int i = 0; i < shipSize; i++) {
                if (isOccupied(row + i, col, player)) {
                    canPlace = false;
                    break;
                }
            }
            if (canPlace) {
                for (int i = 0; i < shipSize; i++) {
                    player[row + i][col] = shipSize;
                }
            }
        }
    }
    return true;
}

void inputGuess(int size, int input[2]) {
    cout << "Enter your guess:\n";
    
    // Input row
    do {
        cout << "Enter row (between 1-" << size << "): ";
        cin >> input[0];
        input[0]--; // Adjust to 0-based index
        if (input[0] >= size || input[0] < 0) {
            cout << "Invalid row input. Please enter a value between 1 and " << size << ".\n";
        }
    } while (input[0] >= size || input[0] < 0);

    // Input column
    do {
        cout << "Enter column (between 1-" << size << "): ";
        cin >> input[1];
        input[1]--; // Adjust to 0-based index
        if (input[1] >= size || input[1] < 0) {
            cout << "Invalid column input. Please enter a value between 1 and " << size << ".\n";
        }
    } while (input[1] >= size || input[1] < 0);
}

bool hitOrMiss(int r, int c, int arr[SIZE][SIZE]) {
    bool hit = false;

    if (arr[r][c] != 0) {
        hit = true;
        arr[r][c] = (arr[r][c] > 1) ? arr[r][c] - 1 : 0;
    }

    if (hit) {
        cout << "Hits\n";
    } else {
        cout << "Misses\n";
    }

    return hit;
}

bool allShipsSunk(int player[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (player[i][j] != 0) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    int pipe1[2];  // From parent to child
    int pipe2[2];  // From child to parent
    int input[2];

    int ships[] = {2, 3, 4};
    int player1[SIZE][SIZE] = {0};
    int player2[SIZE][SIZE] = {0};

    srand(time(0)); // Seed the random number generator

    // Place ships randomly for both players
    for (int shipSize : ships) {
        placeShip(player1, shipSize);
        placeShip(player2, shipSize);
    }

    // Output player grids for debugging purposes (can be removed in actual gameplay)
    output(player1);
    output(player2);

    // Set up pipes
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        return 1;
    }

    // Fork the process to create player 1 and player 2
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid > 0) {  // Parent process (Player 1)
        close(pipe1[0]);  // Close unused read end of pipe1
        close(pipe2[1]);  // Close unused write end of pipe2

        while (true) {
            cout << "Player 1's turn.\n";
            inputGuess(SIZE, input);

            // Send guess to Player 2
            write(pipe1[1], input, sizeof(input));

            // Receive hit/miss response from Player 2
            char response;
            read(pipe2[0], &response, sizeof(response));
            if (response == 'H') {
                cout << "Player 1 hit Player 2's ship!\n";
            } else {
                cout << "Player 1 missed!\n";
            }

            // Read Player 2's guess
            read(pipe2[0], input, sizeof(input));
            bool p2Wins = hitOrMiss(input[0], input[1], player1);
            if (p2Wins) {
                if (allShipsSunk(player1)) {
                    cout << "Player 2 wins!\n";
                    break;
                }
            }

            // Send response to Player 2
            response = p2Wins ? 'H' : 'M';
            write(pipe1[1], &response, sizeof(response));
        }

        close(pipe1[1]);
        close(pipe2[0]);

    } else {  // Child process (Player 2)
        close(pipe1[1]);  // Close unused write end of pipe1
        close(pipe2[0]);  // Close unused read end of pipe2

        while (true) {
            // Read Player 1's guess
            read(pipe1[0], input, sizeof(input));
            bool p1Wins = hitOrMiss(input[0], input[1], player2);
            if (p1Wins) {
                if (allShipsSunk(player2)) {
                    cout << "Player 1 wins!\n";
                    break;
                }
            }

            // Send response to Player 1
            char response = p1Wins ? 'H' : 'M';
            write(pipe2[1], &response, sizeof(response));

            cout << "Player 2's turn.\n";
            inputGuess(SIZE, input);

            // Send guess to Player 1
            write(pipe2[1], input, sizeof(input));

            // Receive hit/miss response from Player 1
            read(pipe1[0], &response, sizeof(response));
            if (response == 'H') {
                cout << "Player 2 hit Player 1's ship!\n";
            } else {
                cout << "Player 2 missed!\n";
            }
        }

        close(pipe1[0]);
        close(pipe2[1]);
    }

    return 0;
}
