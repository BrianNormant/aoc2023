#include <uchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#define DEBUG

#ifdef DEBUG
#define FILE_NAME "example.txt"
#else
#define FILE_NAME "puzzle.txt"
#endif


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

typedef enum {
        PIPE_NS = 0x3, // |
        PIPE_WE = 0xC, // -
        PIPE_NW = 0x5, // ┛
        PIPE_NE = 0x9, // ┗
        PIPE_SW = 0x6, // ┏
        PIPE_SE = 0xA, // ┓
        PIPE_START = 0x10,
        PIPE_VISITED = 0x20,
        PIPE_NEXT = 0x40,
        PIPE_TOP = 0x100,
        PIPE_BOTTOM = 0x200,
        PIPE_LEFT = 0x400,
        PIPE_RIGHT = 0x800,
        GROUND_ENCLOSED = 0x1000,
} pipe_t;

char* PIPE_STR[] = {
        [0] = "O",
        [PIPE_NS] = "┃",
        [PIPE_WE] = "━",
        [PIPE_NW] = "┛",
        [PIPE_NE] = "┗",
        [PIPE_SW] = "┏",
        [PIPE_SE] = "┓",
        [PIPE_START] = "\x1b[31m",
        [PIPE_VISITED] = "\x1b[32m",
        [PIPE_VISITED | PIPE_NEXT ] = "\x1b[32m",
        [PIPE_NEXT] = "\x1b[34m",
        [PIPE_VISITED | PIPE_START] = "\x1b[31m",
};

typedef struct {
        int width;
        int height;
        pipe_t *pipe;
} terrain_t;

void terrain_print(const terrain_t *terrain) {
        // printf("\x1b[2J");
        for (int y = 0; y < terrain->height; y++) {
        
                for (int x = 0; x < terrain->width; x++) {
                        pipe_t p = *(terrain->pipe + terrain->width * y + x);
                        char * color = "";
                        char * pipe = PIPE_STR[p & 0x0F];
                        if ((p & 0x000F) == 0) {
                                if (p & GROUND_ENCLOSED) {
                                        pipe = "I";
                                }
                        }
                        if (p & 0xF0) color = PIPE_STR[p & 0xF0];
                        printf("%s%s\x1b[0m", color, pipe);
                }
                printf("\n");
                fflush(stdout);
        }
}

// Check with direct neighbours and return 1 or 2 valid next pipes
// Check if the neighbours are connected, IE -- or \_, ect.
// Check if the neighbours have already been visited. or are the starting pipe
int pipe_next(terrain_t *terrain, pipe_t *pipe, pipe_t **next1, pipe_t **next2) {
        pipe_t *r,*l,*a,*b; // neighbours

        r = l = a = b = NULL;
        switch (*pipe & 0x00F) { // check edge of the map for neighbours
        case PIPE_NS:
                a = (*pipe & PIPE_TOP)? NULL : pipe - terrain->width;
                b = (*pipe & PIPE_BOTTOM)? NULL : pipe + terrain->width;
                break;
        case PIPE_WE:
                r = (*pipe & PIPE_RIGHT)? NULL : pipe + 1;
                l = (*pipe & PIPE_LEFT)? NULL : pipe - 1;
                break;
        case PIPE_NW:
                a = (*pipe & PIPE_TOP)? NULL : pipe - terrain->width;
                l = (*pipe & PIPE_LEFT)? NULL : pipe - 1;
                break;
        case PIPE_NE:
                a = (*pipe & PIPE_TOP)? NULL : pipe - terrain->width;
                r = (*pipe & PIPE_RIGHT)? NULL : pipe + 1;
                break;
        case PIPE_SW:
                b = (*pipe & PIPE_BOTTOM)? NULL : pipe + terrain->width;
                r = (*pipe & PIPE_RIGHT)? NULL : pipe + 1;
                break;
        case PIPE_SE:
                b = (*pipe & PIPE_BOTTOM)? NULL : pipe + terrain->width;
                l = (*pipe & PIPE_LEFT)? NULL : pipe - 1;
                break;
        }
        if (r) r = (*r & PIPE_VISITED || *r & PIPE_START)? NULL : r;
        if (l) l = (*l & PIPE_VISITED || *l & PIPE_START)? NULL : l;
        if (a) a = (*a & PIPE_VISITED || *a & PIPE_START)? NULL : a;
        if (b) b = (*b & PIPE_VISITED || *b & PIPE_START)? NULL : b;
        if (r) {
                switch(*r & 0x00F) {
                case PIPE_WE:
                case PIPE_NW:
                case PIPE_SE: break;
                default: r = NULL;
                }
        }
        if (l) {
                switch(*l & 0x00F) {
                case PIPE_WE:
                case PIPE_SW:
                case PIPE_NE: break;
                default: l = NULL;
                }
        }
        if (a) {
                switch(*a & 0x00F) {
                case PIPE_NS:
                case PIPE_SE:
                case PIPE_SW: break;
                default: a = NULL;}
        }
        if (b) {
                switch(*b & 0x00F) {
                case PIPE_NS:
                case PIPE_NW:
                case PIPE_NE: break;
                default: b = NULL;
                }
        }
        {
        int valid = 0;
        valid += r!=NULL;
        valid += l!=NULL;
        valid += a!=NULL;
        valid += b!=NULL;
        // printf("valid=%d\n", valid);
        assert(valid == 1 || valid == 0 || valid == 2);
        }

        
        int result = 0;
        if (a) {
                *next1 = a;
                a = NULL;
        } else if (b) {
                *next1 = b;
                b = NULL;
        } else if (r) {
                *next1 = r;
                r = NULL;
        } else if (l) {
                *next1 = l;
                l = NULL;
        } else {
                *next1 = NULL;
        }
        result += *next1 != NULL;
        if (next2) {
                if (a) {
                        *next2 = a;
                        a = NULL;
                } else if (b) {
                        *next2 = b;
                        b = NULL;
                } else if (r) {
                        *next2 = r;
                        r = NULL;
                } else if (l) {
                        *next2 = l;
                        l = NULL;
                } else {
                        *next2 = NULL;
                }
                result += *next2 != NULL;
        }
        
        assert(!a && !b && !r && !l);
        return result;
}

int terrain_follow(terrain_t * terrain, pipe_t *start) {
        pipe_t *next1, *next2, *current1, *current2;;

        int travel = 1;
        next1 = next2 = NULL;

        assert (pipe_next(terrain, start, &next1, &next2) == 2);
        do {
                travel++;
                current1 = next1;
                current2 = next2;

                *current1 |= PIPE_VISITED;
                *current2 |= PIPE_VISITED;

                assert (pipe_next(terrain, current1, &next1, NULL) == 1);
                assert (pipe_next(terrain, current2, &next2, NULL) == 1);

                *next1 |= PIPE_NEXT;
                *next2 |= PIPE_NEXT;
#ifdef DEBUG
                // terrain_print(terrain);
                // usleep(1000 * 500);
#endif
        } while (next1 != next2);
        *start |= PIPE_VISITED;
        *next1 |= PIPE_VISITED;
        
        return travel;
}

int terrain_update_enclosed(terrain_t *terrain) {
        int tile_enclosed = 0;
        int y_wall = 0, x_wall = 0;
        for (int y = 0; y < terrain->height; y++) {
                for (int x = 0; x < terrain->width; x++) {
                        pipe_t *p = (terrain->pipe + terrain->width * y + x);
                        if (*p & 0xF) continue;
                        // count horizontal wall from ground to each side.

                        int horz_wall[2] = {0, 0};
                        int found_me = 0;
                        for (int i = 0; i < terrain->width; i++) {
                                pipe_t *wall = (terrain->pipe + terrain->width * y + i);
                                if (wall == p) { 
                                        found_me = 1;
                                        continue;
                                }
                                if (!(*wall & 0xF)) continue;
                                if (!(*wall & PIPE_VISITED)) continue;
                                if ((*wall & 0xF) == PIPE_WE) continue;
                                
                                horz_wall[found_me]++;
                        }
                        
                        int vert_wall[2] = {0, 0};
                        found_me = 0;
                        for (int i = 0; i < terrain->height; i++) {
                                pipe_t *wall = (terrain->pipe + terrain->width * i + x);
                                if (wall == p) { 
                                        found_me = 1;
                                        continue;
                                }
                                if (!(*wall & 0xF)) continue;
                                if (!(*wall & PIPE_VISITED )) continue;
                                if ((*wall & 0xF) == PIPE_NS) continue;
                                
                                vert_wall[found_me]++;
                        }

                        if (x == 8 && y == 4) {
                                printf("right %d, left %d\n", horz_wall[1], horz_wall[0]);
                                printf("top %d, down %d\n", vert_wall[1], vert_wall[0]);
                        }
                        if ( vert_wall[0] % 2 == 1 && vert_wall[1] % 2 == 1 &&
                             horz_wall[0] % 2 == 1 && horz_wall[1] % 2 == 1) {
                                tile_enclosed++;
                                *p |= GROUND_ENCLOSED;
                                // puts("Enclosed!");
                        }
                }
        }
        return tile_enclosed;
}

void part1(FILE *fp) {
        char line_buf[MAX_LINE_LEN];
        char *line = line_buf;
        char *lines[MAX_LINE_LEN];
        
        int count = 0;
        int result, result2;
        
        while (fgets(line, MAX_LINE_LEN, fp) != NULL) {
                assert(count < MAX_LINE_LEN);

                int len = strlen(line);
                line = strsep(&line, "\n");

                char* line_cpy = malloc(len);
                strcpy(line_cpy, line);

                lines[count++] = line_cpy;
                line = line_buf;
        }
        

        int height, width, connected;
        height = count;
        width = strlen(lines[0]);
        pipe_t *start;
        pipe_t pipe_buf[height][width];
        terrain_t terrain;
        terrain.width = width;
        terrain.height = height;
        terrain.pipe = (pipe_t *) pipe_buf;

        for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                        pipe_t p = 0;

                        p |= (i==0)? PIPE_TOP: 0;
                        p |= (i==height)? PIPE_BOTTOM: 0;
                        p |= (j==0)? PIPE_LEFT: 0;
                        p |= (j==width-1)? PIPE_RIGHT: 0;

                        switch (lines[i][j]) {
                        case '|':
                                p |= PIPE_NS;
                                break;
                        case '-':
                                p |= PIPE_WE;
                                break;
                        case 'J':
                                p |= PIPE_NW;
                                break;
                        case 'L':
                                p |= PIPE_NE;
                                break;
                        case 'F':
                                p |= PIPE_SW;
                                break;
                        case '7':
                                p |= PIPE_SE;
                                break;
                        case 'S':
                                connected = 0;
                                if (i != 0) { // above
                                        switch (lines[i-1][j]) {
                                        case 'F':
                                        case '7':
                                        case '|': connected |= 1;
                                        }
                                }
                                if (i != height - 1) { // below
                                        switch (lines[i+1][j]) {
                                        case 'L':
                                        case 'J':
                                        case '|': connected |= 2;
                                        }
                                }
                                if (j != 0) { // left <-
                                        switch (lines[i][j-1]) {
                                        case 'F':
                                        case 'L':
                                        case '-': connected |= 8;
                                        }
                                }
                                if (j != width - 1) { // right ->
                                        switch (lines[i][j+1]) {
                                        case 'J':
                                        case '7':
                                        case '-': connected |= 4;
                                        }
                                }
                                printf(" ->> %x ", connected);
                                printf("%c\n", lines[i][j+1]);
                                assert(connected == 0x3 ||
                                        connected == 0x5 ||
                                        connected == 0x6 ||
                                        connected == 0x9 ||
                                        connected == 0xA ||
                                        connected == 0xC
                                      );
                                p |= (pipe_t) connected;
                                p |= PIPE_START;
                                start = pipe_buf[i] + j;
                        }
                        pipe_buf[i][j] = p;
                }
        }

        result = terrain_follow(&terrain, start);
        result2= terrain_update_enclosed(&terrain);
        terrain_print(&terrain);

        for (int i = 0; i < count; i++) {
                free(lines[i]);
        }

        printf("Part 1: %d\n Part2 %d\n", result, result2);
}

void part2(FILE *fp) {}
