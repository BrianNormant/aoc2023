#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <limits.h>


// #define FILE_NAME "example.txt"
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
        int x, y;
} point_t;

typedef enum {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        FIRST,
} direction_t;

const direction_t chr_to_dir[] = {
        ['U'] = UP,
        ['D'] = DOWN,
        ['L'] = LEFT,
        ['R'] = RIGHT,
        ['0'] = RIGHT,
        ['1'] = DOWN,
        ['2'] = LEFT,
        ['3'] = UP,
};

uint64_t shoelace_triangle(const point_t * points, int nb_points) {
        uint64_t m00,m01,
                 m10,m11;

        uint64_t twoA = 0;
        for (int i = 0; i < nb_points; i++) {
                m00 = points[i].x;
                m10 = points[i].y;
                m01 = (i != nb_points - 1)? 
                        points[i+1].x: points[0].x;
                m11 = (i != nb_points - 1)?
                        points[i+1].y: points[0].y;

                twoA += m00 * m11;
                twoA -= m01 * m10;
        }
        return twoA / 2;
}

void part1(FILE *fp) {
        char line_buf[MAX_LINE_LEN];
        char *line = line_buf;

        point_t points[1000];
        int nb_points = 0;

        int posx, posy; direction_t last_dir;
        int distance = 0;
        posx = posy = MAX_LINE_LEN;
        last_dir = FIRST;
        
        while (fgets(line, MAX_LINE_LEN, fp) != NULL) {
                int dist;
                char dirc;
                direction_t dir;

                assert( sscanf(line, "%c %d", &dirc, &dist) == 2);
                
                dir = chr_to_dir[(int)dirc];
                if (dir != last_dir) {
                        points[nb_points++] = (point_t){
                                posx,
                                posy,
                        };

                }
                switch (dir) {
                case UP: posx += dist; break;
                case DOWN: posx -= dist; break;
                case LEFT: posy -= dist; break;
                case RIGHT: posy += dist; break;
                default: assert(0);
                }
                distance += dist;
                last_dir = dir;
                line = line_buf;
        }
        assert(posx == MAX_LINE_LEN && posy == MAX_LINE_LEN);

        int solution = shoelace_triangle(points, nb_points);
        solution += distance/2 + 1;
        printf("Solution: %d\n", solution);
}


void part2(FILE *fp) {
        char line_buf[MAX_LINE_LEN];
        char *line = line_buf;

        point_t points[1000];
        int nb_points = 0;

        uint64_t posx, posy; direction_t last_dir;
        uint64_t distance = 0;
        posx = posy = UINT64_MAX/2;
        last_dir = FIRST;
        
        while (fgets(line, MAX_LINE_LEN, fp) != NULL) {
                uint32_t dist;
                char dirc;
                direction_t dir;

                line = strchr(line, '#');

                assert( sscanf(line, "#%5X%c", &dist, &dirc) == 2);
                printf("%c %d\n", dirc, dist);
                dir = chr_to_dir[(int)dirc];
                if (dir != last_dir) {
                        points[nb_points++] = (point_t){
                                posx,
                                posy,
                        };

                }
                switch (dir) {
                case UP: posx += dist; break;
                case DOWN: posx -= dist; break;
                case LEFT: posy -= dist; break;
                case RIGHT: posy += dist; break;
                default: assert(0);
                }
                distance += dist;
                last_dir = dir;
                line = line_buf;
        }

        uint64_t solution = shoelace_triangle(points, nb_points);
        solution += distance/2 + 1;
        printf("Solution: %lu\n", solution);
}
