// https://adventofcode.com/2023/day/1

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int maybeDigit(char c) {
    if (isdigit((unsigned char) c)) {
        return c - '0';
    }
    return -1;
}

// TODO: solve for one O(n) pass next time?
int getTwoDigits(char* line) {
    size_t l, r;
    size_t n = strlen(line);
    l = 0;
    r = n - 1;
    int result = 0;
    int digit;
    while (l <= n - 1) {
        if ((digit = maybeDigit(line[l])) != -1) {
            result = digit * 10;
            break;
        }
        l++;
    }
    while (r >= 0) {
        if ((digit = maybeDigit(line[r])) != -1) {
            result += digit;
            break;
        }
        r--;
    }
    return result;
}

int main() {
    const char *path = "day-01/input.txt";
    FILE *fd;
    if ((fd = fopen(path, "r")) == NULL) {
        perror("Can't open input file");
    }
    const char *line;
    int digits;
    int answer = 0;
    while (fgets(line, 256, fd)) {
        if (strlen(line) == 0) continue;
        digits = getTwoDigits(line);
        // printf("%s -> %d\n", line, digits);
        answer += digits;
    }
    fclose(fd);
    printf("Answer: %d\n", answer);
    return 0;
}
