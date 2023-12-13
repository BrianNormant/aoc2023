#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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

int line_match(const char *line1, const char *line2) {
        return strcmp(line1, line2) == 0;
}

int get_column(char * lines[MAX_LINE_LEN], int nb_line, int index, char * column) {
        int i = 0;
        for (i = 0; i < nb_line ; i++)
                column[i] = lines[i][index];
        column[i] = '\0';
        return 0;
}

// find a matching reflexion by lines
// and columns, store result in matches array
int match_lines(char * patern[MAX_LINE_LEN], int nb_line, int linelen, int matches[10]) {
        int nb_match = 0;
        //lines
        for (int i = 1; i < nb_line; i++) {
                char *line1, *line2;
                int j = 0;
                do {
                        line1 = patern[i-1-j];
                        line2 = patern[i+0+j];
                        if (!line_match(line1, line2)) goto no_v_reflex;
                        j++;
                } while ((i-j-1 >= 0) && (i+j < nb_line));
                matches[nb_match++] = 100 * i;
no_v_reflex:    continue;
        }
        // columns
        for (int i = 1; i < linelen; i++) {
                char column1_buf[MAX_LINE_LEN], column2_buf[MAX_LINE_LEN];
                char *column1 = column1_buf;

                char *column2 = column2_buf;
                int j = 0;
                do {
                        get_column(patern, nb_line, i-1-j, column1);
                        get_column(patern, nb_line, i+j, column2);
                        if (!line_match(column1, column2)) goto no_h_reflex;
                        j++;
                } while ((i-1-j >= 0) && (i+j < linelen));
                
                matches[nb_match++] = i;
no_h_reflex:    continue;
        }

        return nb_match;
}

void part1(FILE *fp) {
        char line_buf[MAX_LINE_LEN];
        char *line = line_buf;
        char *patern[MAX_LINE_LEN];
        int nb_line = 0, linelen = 0;

        int solution = 0;

        while (fgets(line, MAX_LINE_LEN, fp) != NULL) {
                if (line[0] == '\n') {
                        linelen = strlen(patern[0]);
                        
                        int matches[10];
                        assert (match_lines(patern, nb_line, linelen, matches) == 1);
                        solution += matches[0];
                        
                        for(int i = 0; i < nb_line; i++) free(patern[i]);
                        nb_line = 0;
                } else {
                        char *buf = malloc(strlen(line) + 1);
                        strcpy(buf, line);
                        *strchr(buf, '\n') = '\0';
                        patern[nb_line++] = buf;
                }
                line = line_buf;
        }

        printf("Solution: %d\n", solution);
}

void invert(char * patern[MAX_LINE_LEN], int x, int y) {
        switch (patern[y][x]) {
                case '#': patern[y][x] = '.'; return;
                case '.': patern[y][x] = '#'; return;
                default: assert(0);
        }
}
void part2(FILE *fp) {
        char line_buf[MAX_LINE_LEN];
        char *line = line_buf;
        char *patern[MAX_LINE_LEN];
        int nb_line = 0, linelen = 0;

        int solution = 0;

        while (fgets(line, MAX_LINE_LEN, fp) != NULL) {
                if (line[0] == '\n') {
                        linelen = strlen(patern[0]);
                        int origin_match[10],
                            current_match[10];
                        assert (match_lines(patern, nb_line, linelen, origin_match) == 1);
                        // printf(" match to differentiate %d \n", origin_match[0]);
                        int x = 0, y = 0;
                        do {
                                //fix smudge;
                                invert(patern, x, y);

                                int nb_current = match_lines(patern, nb_line, linelen, current_match);

                                // undo last inversion
                                invert(patern, x, y);

                                // printf("[");
                                for (int i = 0; i < nb_current; i++) {
                                        // printf("%d, ", current_match[i]);
                                        if (origin_match[0] != current_match[i]) {
                                                current_match[0] = current_match[i];
                                                goto diff;
                                        }
                                }
                                // printf("]\n");

                                x++;
                                if (x == linelen) {
                                        y++;
                                        x = 0;
                                }                                
                                assert (x < linelen && y < nb_line);
                        } while (1);
diff:
                        // printf("smudge at %d %d\n", x, y);
                        solution += current_match[0];

                        for(int i = 0; i < nb_line; i++) free(patern[i]);
                        nb_line = 0;
                } else {
                        char *buf = malloc(strlen(line) + 1);
                        strcpy(buf, line);
                        *strchr(buf, '\n') = '\0';
                        patern[nb_line++] = buf;
                }
                line = line_buf;
        }

        printf("Solution: %d\n", solution);

}
