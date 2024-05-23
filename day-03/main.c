// https://adventofcode.com/2023/day/3
//
// this helped quite a lot:
// https://www.reddit.com/r/adventofcode/
//
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_null.h>

#define INPUT_LINE_LENGTH 256


// helper function to print out a buffer
void printbuff(char *buffer[3]) {
    for (int i = 0; i < 3; ++i) {
        buffer[i] ? printf("%s", buffer[i]) : printf("(null)\n");
    }
    printf("\n");
}


// checks if the number from [l, r] range from the middle line is adjacent to any symbol
bool checkForSymbols(char *buffer[3], int l, int r) {
    // wow, you can write signed positive values!
    static const int dx[] = {-1, -1, +1, +1, +1, -1, +0, +0};
    static const int dy[] = {-1, +1, -1, +1, +0, +0, +1, -1};

    for (int direction = 0; direction < 8; ++direction) {
        int y = dy[direction] + 1;
        if (buffer[y] == NULL) continue;
        for (int x_offset = l; x_offset <= r; ++x_offset) {
            int x = x_offset + dx[direction];
            if (
                // bound checks
                y >= 0
                && y < 3
                && x >= 0
                && x < strlen(buffer[y])

                // actual logical checks
                && !isdigit(buffer[y][x])
                && buffer[y][x] != '\n'  // it was hard to find actually...
                && buffer[y][x] != '.'
            ) {
                    return true;
            }
        }
    }
    return false;
}


// returns the sum of adjecent numbers from the MIDDLE line in the buffer
int sumAdjacentNumbers(char *buffer[3]) {
    int sum = 0;
    const char *line = buffer[1];
    int length = strlen(line);
    int r = 0;
    printf("> ");
    for (int l = 0; l < length; l++) {
        if (!isdigit(line[l])) continue;
        r = l;
        while (r < length && isdigit(line[r])) {
            r += 1;
        }
        // parse integer (within the r - l range)
        int num = 0;
        int mul = 1;
        for (int z = r - 1; z >= l; z--) {
            num += (int) (line[z] - '0') * mul;  // ascii offset and multiplication
            mul *= 10;
        }
        // r - 1 below for exact check just over/under the right pointer
        bool isadj = checkForSymbols(buffer, l, r - 1);
        if (isadj) sum += num;
        isadj ? printf("+ %d [%d, %d] ", num, l, r) : printf("-> %d [%d, %d] ", num, l, r);
        // shift the left pointer to the right
        l = r - 1;
    }
    return sum;
}


int main() {
    char *path = "day-03/input.txt";
    FILE *fd;
    char line[INPUT_LINE_LENGTH];

    char *buffer[3] = {NULL, NULL, NULL};

    if ((fd = fopen(path, "r")) == NULL) {
        perror("Can't open the input file!");
        return 1;
    }
    int sum_total = 0;

    // start writing to the buffer from the middle
    // so that the begining line is also processed
    int buff_line = 1;

    while (fgets(line, INPUT_LINE_LENGTH, fd) != NULL) {
        // free old line if it exists
        if (buffer[buff_line] != NULL) {
            free(buffer[buff_line]);
        }
        // put a line in the buffer
        buffer[buff_line] = malloc(strlen(line) + 1); // + 1 here because of \0 byte (null termination)
        strcpy(buffer[buff_line], line);

        // process the buffer if it's full
        if (buff_line == 2) {
            printbuff(buffer);
            sum_total += sumAdjacentNumbers(buffer);
            printf("\nRunning total: %d\n\n", sum_total);
            // shift the buffer window
            buffer[0] = buffer[1];
            buffer[1] = buffer[2];
            buffer[2] = NULL;
            buff_line = 2;
        }
        else {
            buff_line += 1;
        }
    }
    printf("\nFinished at buffer line: %d\n\n", buff_line);
    if (buff_line == 2) {
        printbuff(buffer);
        sum_total += sumAdjacentNumbers(buffer);
        printf("\nRunning total: %d\n\n", sum_total);
    }
    // free line buffer on exit
    for (int i = 0; i < 3; i++) {
        if (buffer[i] != NULL) {
            free(buffer[i]);
        }
    }
    // TODO: put these in every C version before, since I definitely forgot about proper file handling...
    fclose(fd);
    printf("Answer (P1): %d\n", sum_total);
    return 0;
}
