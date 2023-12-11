#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "puzzle.txt"
#define MAX_LINE_LEN 1000

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


typedef struct {
        long x;
        long y;
} star_t;
void part1(FILE *fp) {
        char line_buf[MAX_LINE_LEN];
        char *line = line_buf;
        char *galaxy[MAX_LINE_LEN];
        int galaxy_width, galaxy_height;
        
        // parse file and expand empty lines
        galaxy_width = 0;
        while (fgets(line, MAX_LINE_LEN, fp) != NULL) {
                line[strlen(line) - 1] = '\0'; // ignore newline
                galaxy_width = strlen(line); // ignore newline
                
                int expand = 2;
                for (int i = 0; i < galaxy_width; i++) {
                        if (line[i] == '#') {
                                expand = 1;
                                break;
                        }
                }
                
                for (int i = 0; i < expand; i++) {
                        galaxy[galaxy_height] = malloc(sizeof(char)*galaxy_width*2+1);
                        strcpy(galaxy[galaxy_height], line);
                        galaxy_height++;
                }
                line = line_buf;
        }

        // expand empty columns
        for (int x = 0; x < galaxy_width; x++) {
                int expand = 1;
                for (int y = 0; y < galaxy_height; y++) {
                        if (galaxy[y][x] == '#') {
                                expand = 0;
                                break;
                        }
                }
                if (!expand) continue;
                
                galaxy_width++;
                for (int y = 0; y < galaxy_height; y++) {
                        memmove(galaxy[y]+x+sizeof(char), galaxy[y] + x, galaxy_width-x+1);
                        galaxy[y][x] = '.';
                }
                x++;
        }

        // search stars in the galaxy
        star_t stars[MAX_LINE_LEN];
        int star_size = 0;
        for (int y = 0; y < galaxy_height; y++) {
                for (int x = 0; x < galaxy_width; x++)
                        if (galaxy[y][x] == '#') {
                                stars[star_size].x = x;
                                stars[star_size].y = y;
                                star_size++;
                        }
        }
        
        int solution = 0;
        int pairs = 0;
        // calculate distance between stars
        for (int i = 0; i < star_size - 1; i++) {
                for (int j = i + 1; j < star_size; j++) {
                        int dx = stars[i].x - stars[j].x;
                        int dy = stars[i].y - stars[j].y;
                        solution += abs(dx) + abs(dy);
                        pairs++;
                }
        }

        printf("Solution: %d\n", solution);

        for (int i = 0; i < galaxy_height; i++) {
                free(galaxy[i]);
        }
}
void part2(FILE *fp) {
        char line_buf[MAX_LINE_LEN];
        char *line = line_buf;
        char *galaxy[MAX_LINE_LEN];
        int galaxy_width, galaxy_height;
        
        // parse file and expand empty lines
        galaxy_width = 0;
        while (fgets(line, MAX_LINE_LEN, fp) != NULL) {
                line[strlen(line) - 1] = '\0'; // ignore newline
                galaxy_width = strlen(line); // ignore newline
                               
                galaxy[galaxy_height] = malloc(sizeof(char)*galaxy_width*2+1);
                strcpy(galaxy[galaxy_height], line);
                galaxy_height++;
                line = line_buf;
        }

        short expanded_colums[MAX_LINE_LEN];
        short expanded_lines[MAX_LINE_LEN];
        memset(expanded_colums, 0, sizeof(short) * galaxy_height);
        memset(expanded_lines, 0, sizeof(short) * galaxy_width);
        
        // expand empty lines
        for (int y = 0; y < galaxy_height; y++) {
                int expand = 1;
                for (int x = 0; x < galaxy_width; x++) {
                        if (galaxy[y][x] == '#') {
                                expand = 0;
                                break;
                        }
                }
                if (!expand) continue;
                expanded_lines[y] = 1;
        }
        for (int x = 0; x < galaxy_width; x++) {
                int expand = 1;
                for (int y = 0; y < galaxy_height; y++) {
                        if (galaxy[y][x] == '#') {
                                expand = 0;
                                break;
                        }
                }
                if (!expand) continue;
                expanded_colums[x] = 1;
        }

        // for (int y = 0; y < galaxy_height; y++) {
        //         if (expanded_lines[y]) {
        //                 for (int i = 0; i < galaxy_width; i++) {
        //                         putchar('-');
        //                         if (expanded_colums[i]) putchar('+');
        //                 }
        //         } else {
        //                 for (int x = 0; x < galaxy_width; x++) {
        //                         putchar(galaxy[y][x]);
        //                         if (expanded_colums[x]) putchar('|');
        //                 }
        //         }
        //         putchar('\n');
        // }

        // search stars in the galaxy
        // when start is found, adjust the distance by expanded_*;
        const int EXPANSION_FACTOR = 1000000;
        star_t stars[MAX_LINE_LEN];
        int star_size = 0;
        for (int y = 0; y < galaxy_height; y++) {
                for (int x = 0; x < galaxy_width; x++)
                        if (galaxy[y][x] == '#') {
                                long expansion_x = 0;
                                for (int i = 0; i < x; i++) {
                                        expansion_x += expanded_colums[i];
                                }
                                long expansion_y = 0;
                                for (int i = 0; i < y; i++) {
                                        expansion_y += expanded_lines[i];
                                }

                                stars[star_size].x = (x-expansion_x) + expansion_x * EXPANSION_FACTOR;
                                stars[star_size].y = (y-expansion_y) + expansion_y * EXPANSION_FACTOR;
                                star_size++;
                        }
        }

        // calculate distance between stars
        long solution = 0;
        for (int i = 0; i < star_size - 1; i++) {
                for (int j = i + 1; j < star_size; j++) {
                        long dx = stars[i].x - stars[j].x;
                        long dy = stars[i].y - stars[j].y;
                        solution += labs(dx) + labs(dy);
                }
        }

        printf("Solution: %ld\n", solution);


        for (int y = 0; y < galaxy_height; y++) {
                free(galaxy[y]);
        }
}
