#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>


#define FILE_NAME "example.txt"
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
};

int shoelace_triangle(const point_t * points, int nb_points) {
        int m00,m01,
            m10,m11;

        int twoA = 0;
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
                                posx + (posx >= MAX_LINE_LEN?1:0),
                                posy + (posy >= MAX_LINE_LEN?1:0),
                        };

                }
                switch (dir) {
                case UP: posx += dist; break;
                case DOWN: posx -= dist; break;
                case LEFT: posy -= dist; break;
                case RIGHT: posy += dist; break;
                default: assert(0);
                }
                last_dir = dir;
                line = line_buf;
        }
        // assert(posx == MAX_LINE_LEN && posy == MAX_LINE_LEN);

        for (int i = 0; i < nb_points; i++) {
                printf("(%d, %d)\n", points[i].x, points[i].y);
        }

        int solution = shoelace_triangle(points, nb_points);
        printf("Solution: %d\n", solution);
}
void part2(FILE *fp) {}
