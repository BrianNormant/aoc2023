#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#define LINESIZE 1000

enum {
        ONE   = 1,
        TWO   = 2,
        THREE = 3,
        FOUR  = 4,
        FIVE  = 5,
        SIX   = 6,
        SEVEN = 7,
        EIGHT = 8,
        NINE  = 9
};

static const char *DIGITS_STR[] = {
        [0] = "zero",
        [ONE] = "one",
        [TWO] = "two",
        [THREE] = "three",
        [FOUR] = "four",
        [FIVE] = "five",
        [SIX] = "six",
        [SEVEN] = "seven",
        [EIGHT] = "eight",
        [NINE] = "nine"
};

void part1(FILE *f) {
        char line[LINESIZE];
        int result, fdigit, ldigit;
        result = 0;
        
        while (fgets(line, LINESIZE, f)) {
                fdigit = ldigit = 0;
                int len = strlen(line);
                for (int i = 0; i < len; i++) {
                        if (!fdigit && isdigit(line[i])) fdigit = line[i] - '0';
                        if (!ldigit && isdigit(line[len-i-1])) ldigit = line[len-i-1] - '0';
                        if (fdigit && ldigit) break;
                }
                result += fdigit *10 + ldigit;
        }

        printf("sum of all of the calibration values = %d\n", result);
}

void part2(FILE *f) {
        int result = 0;
        char line[LINESIZE+1];
        char word[LINESIZE+1];

        while (fgets(line, LINESIZE, f) != NULL) {
                int fdigit = 0, 
                    ldigit = 0,
                    len = strlen(line);
                for (int i = 0; i < len; i++) {
                        sprintf(word, "%.*s", i, line);
                        for (size_t j = 1; !fdigit && j < 10; j++) 
                                if (strstr(word, DIGITS_STR[j])) {
                                        fdigit = j;
                        }
                        if (!fdigit && isdigit(line[i])) {
                                fdigit = line[i] - '0';
                        }
                        
                        sprintf(word, "%.*s", i, line+len-i-1);
                        for (size_t j = 1; !ldigit && j < 10; j++) 
                                if (strstr(word, DIGITS_STR[j])) {
                                        ldigit = j;
                        }
                        if (!ldigit && isdigit(line[len-i-1])) {
                                ldigit = line[len-i-1] - '0';
                        }
                        if (fdigit && ldigit) break;

                }
                assert(fdigit != -1 && ldigit != -1);
                result += fdigit * 10 + ldigit;
        }
        printf("sum of all of the calibration values = %d\n", result);
}

/* void part2_fast() {
        // same but multiply substring by primenumber and compare to the digit
} */

int main(int argc, char *argv[]) {
        switch (argc) {
                case 2:break;
                default:printf("Usage: %s filename\n", argv[0]);
                        exit(EXIT_FAILURE);
        }

        FILE *f;
        f = fopen(argv[1], "r");
        if (f == NULL) {
                perror("fopen");
                exit(EXIT_FAILURE);
        }
        part1(f);
        fclose(f);
        
        f = fopen(argv[1], "r");
        if (f == NULL) {
                perror("fopen");
                exit(EXIT_FAILURE);
        }
        part2(f);
        fclose(f);
}
