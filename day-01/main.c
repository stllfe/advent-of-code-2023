// https://adventofcode.com/2023/day/1

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINE_SIZE 256
#define N_NUMBERS 9

const char *WORDS[N_NUMBERS] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

char maybeWord(char* chars) {
    for (int i = 0; i < N_NUMBERS; ++i)
    {
        if (strstr(chars, WORDS[i]) != NULL) {
            return i + 1;
        }
    }
    return -1;
}

char maybeDigit(char c) {
    if (isdigit((unsigned char) c)) {
        return c - '0';
    }
    return -1;
}

char *getSubStr(const char * src, size_t len) {
    char *sub = malloc(len);
    if (!sub) {
        perror("Can't allocate a substring!");
        exit(1);
    }
    strncpy(sub, src, len);
    sub[len - 1] = '\0';  // null termination
    return sub;
}

// TODO: improve time complexity?
unsigned char getTwoDigits(char* line) {
    unsigned int l = 0, r = strlen(line) - 1;
    unsigned char result = 0;
    char digit;
    while (l <= r) {
        // that's a hacky way to get a leftmost substring
        // TODO: are there a better alternatives?
        char *subline = getSubStr(line, l + 1);
        // printf("%s\n", sub);
        char w = maybeWord(subline);
        char d = maybeDigit(line[l]);
        free(subline);
        if ((digit = w != -1 ? w : d) != -1)
        {
            result = (unsigned char) digit * 10;
            break;
        }
        l++;
    }
    while (r >= 0) {
        char w = maybeWord(&line[r]);
        char d = maybeDigit(line[r]);
        if ((digit = w != -1 ? w : d) != -1) {
            result += (unsigned char) digit;
            break;
        }
        r--;
    }
    return result;
}

int main() {
    const char *path = "day-01/input.txt";
    FILE *fd;
    char line[MAX_LINE_SIZE];

    if ((fd = fopen(path, "r")) == NULL) {
        perror("Can't open input file");
        return 1;
    }

    int answer = 0;
    while (fgets(line, MAX_LINE_SIZE, fd) != NULL) {
        if (strlen(line) == 0)
            continue;
        unsigned char digits = getTwoDigits(line);
        //printf("%s -> %d\n", line, digits);
        answer += digits;
    }
    fclose(fd);
    printf("Answer: %d\n", answer);
    return 0;
}
