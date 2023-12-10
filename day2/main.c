#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>

#define LINE_MAX_LEN 1000

#define MIN(a, b) (((a) < (b))?(a):(b))
#define MAX(a, b) (((a) > (b))?(a):(b))

void part1(FILE *fp);

void part2(FILE *fp);

int main(int argc, char *argv[]) {
        switch (argc) {
                case 2: break;

                default: printf("Usage: %s <file>\n", argv[0]);
                         exit(1);
        }
        FILE *fp;
        if ((fp = fopen(argv[1], "r")) == NULL) {
                perror("fopen");
                exit(1);
        }
        part1(fp);
        fclose(fp);

        if ((fp = fopen(argv[1], "r")) == NULL) {
                perror("fopen");
                exit(1);
        }
        part2(fp);
        fclose(fp);
}


static enum {
        RED = 0,
        BLUE,
        GREEN
};

static const char *COLORS[] = {
        [RED] = "red",
        [BLUE] = "blue",
        [GREEN] = "green"
};

void part1(FILE *fp) {
        char line_buf[LINE_MAX_LEN+1];
        char *line = line_buf;
        int sumID = 0;

        static const int MAX_RED = 12;
        static const int MAX_GREEN = 13;
        static const int MAX_BLUE = 14;
        
        while (fgets(line, LINE_MAX_LEN, fp) != NULL) {
                int red, green, blue, id;
                char *tok, *line_parsed, *save_ptr1, *save_ptr2;
                
                red = green = blue = 0;
                save_ptr1 = save_ptr2 = NULL;
                line_parsed = line;

                sscanf(line, "Game %d:", &id);

                (void) strsep(&line_parsed, ":");
                while ((tok = strtok_r(line_parsed, ";", &save_ptr1)) != NULL) {
                        line_parsed = NULL;
                        
                        int tmp;
                        char *stok, buf[10];
                        while ((stok = strtok_r(tok, ",", &save_ptr2)) != NULL) {
                                tok = NULL;
                                sscanf(stok, "%d %[^;,\n\r]9s", &tmp, buf);
                                for (unsigned int i = 0; i < 3; i++) {
                                        if (strncmp(buf, COLORS[i], 9) == 0) {
                                                switch (i) {
                                                case RED: red = tmp; break;
                                                case GREEN: green = tmp; break;
                                                case BLUE: blue = tmp; break;
                                                }
                                        }
                                }
                        }
                        if (   red   > MAX_RED 
                            || green > MAX_GREEN 
                            || blue  > MAX_BLUE) {
                                goto impgame; // Game is impossible, skip it.
                        }
                }
                sumID += id;
impgame:
        }
        printf("sumID = %d\n", sumID);
}

void part2(FILE *fp) {
        char line_buf[LINE_MAX_LEN+1],
            *line = line_buf;
        int sumPow = 0;
        
        while (fgets(line, LINE_MAX_LEN, fp) != NULL) {
                int red_min, blue_min, green_min;
                char *tok, *line_parsed;

                line_parsed = line;
                red_min = blue_min = green_min = INT_MIN;

                strsep(&line_parsed, ":");

                while ((tok = strsep(&line_parsed, ";,"))) {
                        int n; char buf[32+1];

                        if (sscanf(tok, "%d %32[^;,\n\r]s", &n, buf) != 2) 
                                continue;
                        for (uint32_t i = 0; i < 3; i++) {
                                if (strncmp(COLORS[i], buf, 32) == 0) {
                                        switch (i) {
                                        case RED:
                                                red_min = MAX(red_min, n);
                                                break;
                                        case GREEN:
                                                green_min = MAX(green_min, n);
                                                break;
                                        case BLUE:
                                                blue_min = MAX(blue_min, n);
                                                break;
                                        }
                                        break;
                                }
                        }

                }
                sumPow += red_min*blue_min*green_min;
                
        }
        printf("sumPow = %d\n", sumPow);
}
