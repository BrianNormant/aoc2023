#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#define FILE_NAME "puzzle.txt"
#define MAX_LINE_LEN 1000
#define MAX_TRACE 5000
void part1(FILE *fp);
void part2(FILE *fp);

int main(void) {
        FILE *fp;

        fp = fopen(FILE_NAME, "r");
        if (fp == NULL) {
                perror("fopen");
                exit(1);
        }
        part1(fp);
        fclose(fp);


        fp = fopen(FILE_NAME, "r");
        if (fp == NULL) {
                perror("fopen");
                exit(1);
        }
        part2(fp);
        fclose(fp);

        return 0;
}

#define MAX_SIZE 150

typedef enum {
        ROUND,
        SQUARE,
        NONE
} rock_t;


void tilt_north(rock_t rocks[MAX_SIZE][MAX_SIZE], int x, int y) {
        for (int i = 1; i < y; i++) {
                for (int j = 0; j < x; j++) {
                        if (rocks[i][j] != ROUND) continue;
                        if (rocks[i-1][j] != NONE) continue;
                        rocks[i][j] = NONE;
                        rocks[i-1][j] = ROUND;
                }
        }
}

void tilt_south(rock_t rocks[MAX_SIZE][MAX_SIZE], int x, int y) {
        for (int i = y - 2; i >= 0; i--) {
                for (int j = 0; j < x; j++) {
                        if (rocks[i][j] != ROUND) continue;
                        if (rocks[i+1][j] != NONE) continue;
                        rocks[i][j] = NONE;
                        rocks[i+1][j] = ROUND;
                }
        }
}

// move to the right ->
void tilt_west(rock_t rocks[MAX_SIZE][MAX_SIZE], int x, int y) {
        for (int i = 0; i < y; i++) {
                for (int j = x - 2; j >= 0; j--) {
                        if (rocks[i][j] != ROUND) continue;
                        if (rocks[i][j+1] != NONE) continue;
                        rocks[i][j] = NONE;
                        rocks[i][j+1] = ROUND;
                }
        }
}
void tilt_east(rock_t rocks[MAX_SIZE][MAX_SIZE], int x, int y) {
        for (int i = 0; i < y; i++) {
                for (int j = 1; j < x; j++) {
                        if (rocks[i][j] != ROUND) continue;
                        if (rocks[i][j-1] != NONE) continue;
                        rocks[i][j] = NONE;
                        rocks[i][j-1] = ROUND;
                }
        }
}

void rocks_print(rock_t rocks[MAX_SIZE][MAX_SIZE], int x, int y) {
        for (int i = 0; i < y; i++) {
                for (int j = 0; j < x; j++) {
                        char c;
                        switch (rocks[i][j]) {
                                case ROUND:
                                        c = 'O';
                                        break;
                                case SQUARE:
                                        c = '#';
                                        break;
                                case NONE:
                                        c = '.';
                                        break;
                        }
                        printf("%c", c);
                }
                printf("\n");
        }
}


int rocks_cmp(rock_t a[MAX_SIZE][MAX_SIZE], rock_t b[MAX_SIZE][MAX_SIZE], int x, int y) {
        for (int i = 0; i < y; i++) {
                for (int j = 0; j < x; j++) {
                        if (a[i][j] != b[i][j]) return 1;
                }
        }
        return 0;
}

void rocks_cpy(rock_t dst[MAX_SIZE][MAX_SIZE], rock_t src[MAX_SIZE][MAX_SIZE], int x, int y) {
        for (int i = 0; i < y; i++) {
                memcpy(dst[i], src[i], x * sizeof(rock_t));
        }
}

// roll north, west, south, east
void cycle(rock_t rocks[MAX_SIZE][MAX_SIZE], int x, int y) {
        rock_t rocks_before[MAX_SIZE][MAX_SIZE];
        int i = 0;
        int c = 0;
        do {
                c++;
                rocks_cpy(rocks_before, rocks, x, y);
                switch (i) {
                case 0: tilt_north(rocks, x, y); break;
                case 1: tilt_east( rocks, x, y); break;
                case 2: tilt_south(rocks, x, y); break;
                case 3: tilt_west( rocks, x, y); break;
                }
                if (rocks_cmp(rocks, rocks_before, x, y) == 0) i++;
        } while (i < 4);
        // printf("Took %d iteration to cycle\n", c);
}


void part1(FILE *fp) {
        char line_buf[MAX_LINE_LEN];
        char *line = line_buf;

        rock_t rocks[MAX_SIZE][MAX_SIZE],
               rocks_before[MAX_SIZE][MAX_SIZE];


        int x = 0, y = 0;
        while (fgets(line, MAX_LINE_LEN, fp) != NULL) {
                x = strlen(line) - 1;
                line[x] = '\0';
                for (int i = 0; i < x; i++) {
                        switch (line[i]) {
                        case 'O': rocks[y][i] = ROUND; break;
                        case '#': rocks[y][i] = SQUARE; break;
                        case '.': rocks[y][i] = NONE; break;
                        default: assert(0);
                        }
                }
                y++;
        }

        // rocks_print(rocks, x, y);
        do {
                rocks_cpy(rocks_before, rocks, x, y);
                tilt_north(rocks, x, y);
        } while (rocks_cmp(rocks, rocks_before, x, y) != 0);
        // puts("--------------------");
        // rocks_print(rocks, x, y);
        
        int solution = 0;
        for (int i = 0; i < y; i++) {
                for (int j = 0; j < x; j++) {
                        if (rocks[i][j] != ROUND) continue;
                        solution += y - i;
                }
        }
        
        printf("Solution: %d\n", solution);
}

int rocks_load(rock_t rocks[MAX_SIZE][MAX_SIZE], int x, int y) {
        int load = 0;
        for (int i = 0; i < y; i++) {
                for (int j = 0; j < x; j++) {
                        if (rocks[i][j] != ROUND) continue;
                        load += y - i;
                }
        }
        return load;
}

void cycle_print(const int *trace, int trace_size, int index) {
        printf("Trace: [");
        for (int i = 0; i < trace_size; i++) {
                if (i == trace_size - (trace_size - index) - (trace_size - index)) printf("\x1b[33m");
                if (i == index) printf("\x1b[32m");
                printf("%d, ", trace[i]);
        }
        printf("\x1b[0m]\n");
}

int cycle_cmp(const int *a, const int *b, int n) {
        for (int i = 0; i < n; i++) {
                if (a[i] != b[i]) return 1;
        }
        return 0;
}

int cycle_index(const int *trace, int trace_size) {
        if (trace_size < 4) return -1;
        // printf("Search for subindex\n");
        for (int i = 2; i <= trace_size / 2; i++) {
                const int * a = trace + trace_size - i;
                const int * b = a - i;
                // printf("Comparing"); cycle_print(a, i, 0);
                // printf("To "); cycle_print(b, i, 0);
                // assert (a >= trace); assert (b >= trace);
                if (cycle_cmp(a, b, i) == 0)
                        return trace_size - i;
        }
        return -1;
}


void part2(FILE *fp) {
        char line_buf[MAX_LINE_LEN];
        char *line = line_buf;

        rock_t rocks[MAX_SIZE][MAX_SIZE];


        int x = 0, y = 0;
        while (fgets(line, MAX_LINE_LEN, fp) != NULL) {
                x = strlen(line) - 1;
                line[x] = '\0';
                for (int i = 0; i < x; i++) {
                        switch (line[i]) {
                        case 'O': rocks[y][i] = ROUND; break;
                        case '#': rocks[y][i] = SQUARE; break;
                        case '.': rocks[y][i] = NONE; break;
                        default: assert(0);
                        }
                }
                y++;
        }

        int trace[MAX_TRACE];
        int trace_len = 0;
        int index;
        for (long i = 0; ; i++) {
                cycle(rocks, x, y);

                int load = rocks_load(rocks, x, y);
                // cycle_print(trace, trace_len, 0);
                if ((index = cycle_index(trace, trace_len)) != -1) {
                        cycle_print(trace, trace_len, index);
                        break;
                }
                
                assert(trace_len <= MAX_LINE_LEN);
                trace[trace_len++] = load;
                // printf("\n");
        }

        int solution = 0;
        int a = trace_len - index;
        int b = trace_len - ( trace_len - index ) - ( trace_len - index );
        solution = trace[(999999999 - b)%a+b];
        // for (int i = b; i < 21; i++ ) {
        //         printf("%d : [%d] = %d\n", i+1, (i-b)%a+b, trace[(i-b)%a+b]);
        // }
        printf("Solution: %d\n", solution);
}
