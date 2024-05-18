// https://adventofcode.com/2023/day/3

// Инициализируем некоторый буффер строк первыми тремя строчками.

// Идём по строчке слева-направо с помощью двух указателей.
// Пока мы не внутри числа, отматываем левым. Как только встретили границу числа, начинаем отодвигать правый указатель до его границы.
// Как только закончили вычитку числа, проверяем из трёх считанных строчек линейным поиском окружающие символы. Если попадаем в символ отличающийся от точки или цифры,
// считаем, что число `Adjacent` и добавляем его в общую сумму, иначе, пропускаем его.

// Как только строчка закончилась, переходим на следующую и проверяем доступна ли ещё одна строчку снизу от неё: если ещё не считали, то добавляем её в буфер, иначе просто используем.

// Повторяем процедуру до следующей строчки пока при очередном завершении строчки не считываем \EOF
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_LINE_LENGTH 256


// TODO finish this function
void sumAdjacentNumbers(char *buffer[3]) {
    int sum = 0;
    for (int i = 0; i < 3; i++) {
        const char *line = buffer[i];
        int length = strlen(line);
        int r = 0;
        for (int l = 0; l < length; l++) {
            if (!isdigit(line[l])) continue;
            r = l;
            while (r < length && isdigit(line[r])) {
                r += 1;
            }
            int span_len = r - l;
            char *slice = malloc(span_len + 1);
            strncpy(slice, line + l, span_len);
            slice[span_len] = '\0';
            printf("-> %s ", slice);
            l = r - 1;
            free(slice);
        }
    }
}


int main() {
    char *path = "day-03/mini-input.txt";
    FILE *fd;
    char line[INPUT_LINE_LENGTH];

    char *buffer[3] = {NULL, NULL, NULL};

    if ((fd = fopen(path, "r")) == NULL) {
        perror("Can't open the input file!");
        return 1;
    }
    int curr_line = 0;
    while (fgets(line, INPUT_LINE_LENGTH, fd) != NULL) {
        // free old line if it exists
        if (buffer[curr_line] != NULL) {
            free(buffer[curr_line]);
        }
        // put a line in the buffer
        buffer[curr_line] = malloc(strlen(line) + 1); // + 1 here because of \0 byte (null termination)
        strcpy(buffer[curr_line], line);

        // process the buffer if it's full
        if (curr_line == 2) {
            for (int i = 0; i < 3; ++i) {
                printf("%s", buffer[i]);
            }
            printf("\n\n");
            sumAdjacentNumbers(buffer);
            curr_line = 0;
        }
        else {
            curr_line += 1;
        }
    }
    // free line buffer on exit
    for (int i = 0; i < 3; i++) {
        if (buffer[i] != NULL) {
            free(buffer[i]);
        }
    }

    // TODO: put these in every C version before, since I definitely forgot about proper file handling...
    fclose(fd);
    return 0;
}
