#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#define FILE_NAME "puzzle.txt"
#define MAX_LINE_LEN 1000
#define MAX_LASER 2000

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

typedef enum {
        TILE_EMPTY  = 0x00, // .
        TILE_SPLITH = 0x01, // -
        TILE_SPLITV = 0x02, // |
        TILE_MIRRN  = 0x04, // /
        TILE_BMIRR  = 0x08, /* \ */
        TILE_LIT    = 0x1000,
} tile_t;

const char tile_to_chr[] = {
        [TILE_EMPTY] = '.',
        [TILE_SPLITH] = '-',
        [TILE_SPLITV] = '|',
        [TILE_MIRRN] = '/',
        [TILE_BMIRR] = '\\',
};

typedef struct {
        short x, y;
        short sx, sy;
        char dx, dy;
        char sdx, sdy;
        char r;
} laser_t;

void map_print(tile_t map[MAX_LINE_LEN * MAX_LINE_LEN], int x, int y) {
        printf("\x1b[2J \x1b[1;1H");
        for (int i = 0; i < x; i++) {
                for (int j = 0; j < y; j++) {
                        tile_t t = map[i * y + j];
                        if (t & TILE_LIT) printf("\x1b[42m");
                        putchar(tile_to_chr[t & 0x0F]);
                        printf("\x1b[0m");
                }
                printf("\n");
        }
}

// test the number of illuminated tile if the laser start at sx sy going dx + dy
int test_configuration(tile_t map[MAX_LINE_LEN / MAX_LINE_LEN], int x, int y, laser_t start) {
        
        // clean illuminated tiles from last round;
        for (int i = 0 ; i < x * y; i++) map[i] &= 0xF;

        laser_t lasers[MAX_LASER];
        memset(lasers, 0, sizeof(lasers));
        lasers[0] = start;
        int laser_size = 1;
        int active_laser = 1;

        while (active_laser > 0) {
                int l_c = laser_size;
                active_laser = 0;
                for (int i = 0; i < l_c; i++) {
                        int invert = 1;
                        laser_t * laser = lasers + i;
                        //
                        if (laser->r == 2) continue;
                        // if the laser is outside the map or inactive, ignore it.
                        if ( (!laser->dx && !laser->dy) ||
                              laser->x < 0 || laser->x >= x ||
                              laser->y < 0 || laser->y >= y) continue;
                        // if the laser is reach its start, ignore, it is looping, ignore it.
                        if (laser->x == laser->sx && 
                            laser->y == laser->sy &&
                            laser->dy == laser->sdy &&
                            laser->dx == laser->sdx &&
                            laser->r) continue;

                        tile_t *t = &map[laser->y * x + laser->x];
                        if (*t & TILE_LIT) {
                                if (( laser->dx && *t & TILE_SPLITV ) || 
                                    ( laser->dy && *t & TILE_SPLITH )) {
                                        laser->r = 2;
                                        continue;
                                }
                        }


                        laser->r = 1;

                        active_laser++;
                        *t |= TILE_LIT;

                        assert (abs(laser->dx) + abs(laser->dy) == 1);
                        // react to the tile and update the laser positon & direction.
                        switch (map[laser->y * x + laser->x] & 0x0F) {
                        case TILE_SPLITV:
                                if (!laser->dx) {
                                        laser->y += laser->dy;
                                        break; 
                                }
                                laser_t u,d;
                                u = (laser_t){ .y = laser->y - 1, .sy = laser->y - 1,
                                      .x = laser->x, .sx = laser->x,
                                      .dy = -1, .sdy = -1,};
                                d = (laser_t){.y = laser->y + 1, .sy = laser->y + 1,
                                     .x = laser->x, .sx = laser->x,
                                     .dy =  1, .sdy =  1,};
                                *laser = u;
                                lasers[laser_size++] = d;
                                break;
                        case TILE_SPLITH:
                                if (!laser->dy) {
                                        laser->x += laser->dx;
                                        break; 
                                }
                                laser_t r,l;
                                r = (laser_t){ .y = laser->y, .sy = laser->y, 
                                      .x = laser->x - 1, .sx = laser->x - 1,
                                      .dx = -1, .sdx = -1,
                                      .dy = 0, .sdy = 0};
                                l = (laser_t){.y = laser->y, .sy = laser->y,
                                     .x = laser->x + 1, .sx = laser->x + 1,
                                     .dx = 1, .sdx = 1,
                                     .dy = 0, .sdy = 0};
                                *laser = l;
                                lasers[laser_size++] = r;
                                break;
                        case TILE_MIRRN: invert = -1;
                                         // fall through
                        case TILE_BMIRR:
                                if (laser->dx == -1 * invert) {
                                        laser->dx =  0;
                                        laser->dy = -1;
                                        laser->y--;
                                } else if (laser->dx == 1 * invert) {
                                        laser->dx = 0; 
                                        laser->dy = 1;
                                        laser->y++;
                                } else if (laser->dy == -1 * invert) {
                                        laser->dx = -1;
                                        laser->dy = 0;
                                        laser->x--;
                                } else if (laser->dy == 1 * invert) {
                                        laser->dx = 1;
                                        laser->dy = 0;
                                        laser->x++;
                                }
                                break;
                        case TILE_EMPTY:
                                laser->x += laser->dx;
                                laser->y += laser->dy;
                                break;
                        default: assert(0);
                        }
                }
                printf("active_laser = %d\n", active_laser);
                usleep(50000);
                map_print(map, x, y);
        }

        int solution = 0;
        for (int i = 0 ; i < x * y; i++) solution += (map[i] & TILE_LIT) == TILE_LIT;
        return solution;
}

void part1(FILE *fp) {
        char line_buf[MAX_LINE_LEN];
        char *line = line_buf;

        int solution = 0;

        tile_t map[MAX_LINE_LEN * MAX_LINE_LEN] = { 0 };
        int x,y;

        y = 0;
        while (fgets(line, MAX_LINE_LEN, fp) != NULL) {

                x = strlen(line);
                line[x - 1] = '\0'; x--;

                for (int i = 0 ; i < x; i++) {
                        switch (line[i]) {
                        case '.' : map[y * x + i] = TILE_EMPTY; break;
                        case '-' : map[y * x + i] = TILE_SPLITH; break;
                        case '|' : map[y * x + i] = TILE_SPLITV; break;
                        case '/' : map[y * x + i] = TILE_MIRRN; break;
                        case '\\': map[y * x + i] = TILE_BMIRR; break;
                        default: assert(0);
                        }
                }

                y++;
                line = line_buf;
        }

        laser_t start = { .dx = 1, .sdx = 1,};
        solution = test_configuration(map, x, y, start);

        printf("Solution: %d\n", solution);
}

int array_max(int *arr, int len) {
        int max = arr[0];
        for (int i = 1; i < len; i++) max = max > arr[i]? max : arr[i];
        return max;
}

void part2(FILE *fp) {
        char line_buf[MAX_LINE_LEN];
        char *line = line_buf;

        int solution = 0;

        tile_t map[MAX_LINE_LEN * MAX_LINE_LEN] = { 0 };
        int x,y;

        y = 0;
        while (fgets(line, MAX_LINE_LEN, fp) != NULL) {

                x = strlen(line);
                line[x - 1] = '\0'; x--;

                for (int i = 0 ; i < x; i++) {
                        switch (line[i]) {
                        case '.' : map[y * x + i] = TILE_EMPTY; break;
                        case '-' : map[y * x + i] = TILE_SPLITH; break;
                        case '|' : map[y * x + i] = TILE_SPLITV; break;
                        case '/' : map[y * x + i] = TILE_MIRRN; break;
                        case '\\': map[y * x + i] = TILE_BMIRR; break;
                        default: assert(0);
                        }
                }

                y++;
                line = line_buf;
        }

        int illuminated[MAX_LINE_LEN * 4];
        int illuminated_s = 0;
        for (int i = 0; i < x; i++) {
                laser_t start = (laser_t) {
                        .x = i, .sx = i,
                        .dy = 1, .sdy = 1
                };
                // top row
                illuminated[illuminated_s++] = test_configuration(map, x, y, start);
                start.y = y-1; start.sy = y-1;
                start.dy = -1, start.sdy = -1;
                // bottom row
                illuminated[illuminated_s++] = test_configuration(map, x, y, start);
        }
        for (int i = 0; i < y; i++) {
                laser_t start = (laser_t) {
                        .y = i, .sy = i,
                        .dx = 1, .sdx = 1,
                };
                // left column
                illuminated[illuminated_s++] = test_configuration(map, x, y, start);
                start.x = x-1; start.sx = x-1;
                start.dx = -1, start.sdx = -1;
                // right column
                illuminated[illuminated_s++] = test_configuration(map, x, y, start);
        };

        solution = array_max(illuminated, illuminated_s);

        printf("Solution: %d\n", solution);

}
