#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_R_CUBES 12
#define NUM_G_CUBES 13
#define NUM_B_CUBES 14
#define INPUT_LINE_LENGTH 256

struct Turn {
    int red;
    int green;
    int blue;

    // make it a linked list
    struct Turn *next;
};

struct Game {
    int id;
    struct Turn turn;
};

struct Turn *getFewestNumberOfCubesPossible(struct Turn *turn) {
    struct Turn *t = malloc(sizeof(struct Turn));
    if (!t) {
        perror("Can't allocate memory for a resulting turn node!");
        exit(EXIT_FAILURE);
    }
    t->red = t->green = t->blue = 0;
    while (turn) {
        t->red = turn->red > t->red ? turn->red : t->red;
        t->green = turn->green > t->green ? turn->green : t->green;
        t->blue = turn->blue > t->blue ? turn->blue : t->blue;
        turn = turn->next;
    }
    return t;
}

bool isPossible(struct Game *game) {
    struct Turn *turn = &game->turn;
    while (turn) {
        if ((turn->red > NUM_R_CUBES) || (turn->blue > NUM_B_CUBES) ||
            (turn->green > NUM_G_CUBES)) {
            return false;
        }
        turn = turn->next;
    }
    return true;
}

void fillInTurnInfo(char *color_str, struct Turn *turn) {
    char *c = color_str;
    int v = strtod(c, &c);
    c += strspn(c, " ");
    // printf("   color=%s\tval=%d\n", c, v);
    if (strcmp(c, "red") == 0) {
        turn->red = v;
    } else if (strcmp(c, "green") == 0) {
        turn->green = v;
    } else if (strcmp(c, "blue") == 0) {
        turn->blue = v;
    } else {
        fprintf(stderr, "Can't parse color string: %s\n", color_str);
        exit(EXIT_FAILURE);
    }
}

// Parses strings like:
// Game 1: 14 green, 8 blue, 9 red; 5 blue, 4 green, 2 red
//
// TODO: I almost don't get a thing of what am I doing here, but it works!
// followed this:
// https://stackoverflow.com/questions/70820551/efficient-string-parsing-in-c
// TODO: get a sence of this low-level string pointers manipulation...
void parseGame(char *line, struct Game *game) {
    static const char *space = " ";
    static const char *game_sep = ":";
    static const char *turn_sep = ";\n";
    static const char comma = ',';

    // parse game id first
    char *game_str = line;
    line += strcspn(line, game_sep);
    if (*line) {
        *line++ = 0;
    } else {
        perror("Incorrect game string!");
        exit(1);
    }
    game_str += strcspn(game_str, space);
    if (*game_str) {
        *game_str++ = 0;
    } else {
        perror("Incorrect game string!");
        exit(EXIT_FAILURE);
    }
    // TODO can we handle an error here just in case?
    game->id = strtod(game_str, &game_str);
    // printf("Parsed game ID: %d\n", id);
    struct Turn *turn = &game->turn;
    while (*line) {
        // set turn info with zeros by default
        turn->red = turn->green = turn->blue = 0;
        // skip leading whitespace
        line += strcspn(line, space);
        if (!*line) {
            // that was actually trailing whitespace at the end of the string
            break;
        }
        // parse turn strings
        char *turn_str = line;
        line = strpbrk(line, turn_sep);
        if (!line) {
            // do we really need to raise here?
            fputs("Missing turn separators\n", stderr);
            exit(EXIT_FAILURE);
        }
        *line++ = 0;  // terminate turn
        // printf("turn string: %s\n", turn_str);
        while (*turn_str) {
            // parse turn colors
            char *color_str = turn_str;
            turn_str = strchr(turn_str, comma);
            if (!turn_str) {
                // skip leading whitespace
                // printf("  color string: %s\n", color);
                fillInTurnInfo(color_str, turn);
                break;
            }
            *turn_str++ = 0;
            turn_str += strcspn(turn_str, space);
            fillInTurnInfo(color_str, turn);
        }
        // link to next turn
        turn->next = malloc(sizeof(struct Turn));
        if (turn->next == NULL) {
            perror("Can't allocate memory to turns linked list node!");
            exit(EXIT_FAILURE);
        }
        turn = turn->next;
    }
    turn->next = NULL;
}

int main() {
    const char *path = "day-02/input.txt";
    FILE *fd;
    char line[INPUT_LINE_LENGTH];

    if ((fd = fopen(path, "r")) == NULL) {
        perror("Can't open input file");
        return 1;
    }

    int part_1_answer = 0;
    int part_2_answer = 0;
    while (fgets(line, INPUT_LINE_LENGTH, fd) != NULL) {
        struct Game game;
        parseGame(line, &game);
        if (isPossible(&game)) part_1_answer += game.id;
        struct Turn turn = *getFewestNumberOfCubesPossible(&game.turn);
        part_2_answer += turn.red * turn.green * turn.blue;
    }
    printf("(P1) Answer: %d\n", part_1_answer);
    printf("(P2) Answer: %d\n", part_2_answer);
}
