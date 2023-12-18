#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>

#define MAX_LINE_SIZE 1000
#define MAX_SUB_STR 10


void part1(FILE *fp);
void part2(FILE *fp);

// ** File is always 3+ lines
int main(int argc, char *argv[]) {

        if ( argc != 2) {
                fprintf(stderr, "Usage: %s <input>\n", argv[0]);
                argv[1] = "input.txt";
        }
        FILE *fp;

        fp = fopen(argv[1], "r");
        if (fp == NULL) {
                perror("fopen");
                exit(1);
        }
        part1(fp);
        fclose(fp);


        fp = fopen(argv[1], "r");
        if (fp == NULL) {
                perror("fopen");
                exit(1);
        }
        part2(fp);
        fclose(fp);

        return 0;
}

#pragma GCC diagnostic ignored "-Wunused-function"
static void
pretty_print(char *restrict cur, char *restrict above, char *restrict below, int size, int index) {
int len, start, end;

        sscanf(cur+index, "%*d%n", &len);
        
        assert(len != 0);

        if (index != 0) {
                start = -1;
        } else {
                start = 0;
        }
        
        if (index + len != size) {
                end = len + 1;
        } else {
                end = len;
        }

        // if (above) printf("\t|%.*s|\n", end-start, above + index + start);
        // if (cur)   printf("\t|%.*s|\n", end-start, cur   + index + start);
        // if (below) printf("\t|%.*s|\n", end-start, below + index + start);
}


static int 
issymbol(char c) { 
        if ((c != '.') && !isdigit(c)) {
                return true;
        }
        return false;
}

/**
  * above and below may be NULL
  */
static int
check_around(char *restrict cur, char *restrict above, char *restrict below, int size, int index) {
        int len, start, end;

        sscanf(cur+index, "%*d%n", &len);
        
        assert(len != 0);

        if (index != 0) {
                start = -1;
                if (issymbol(cur[index-1])) return true;
        } else {
                start = 0;
        }
        
        if (index + len != size) {
                end = len + 1;
                if (issymbol(cur[index+len])) return true;
        } else {
                end = len;
        }

        for (int i = start; i < end; i++) {
                if (above) if (issymbol(above[index+i])) return true;
                if (below) if (issymbol(below[index+i])) return true;
        }

        return false;
}
void 
part1(FILE *fp) {
        char above_l_buffer [MAX_LINE_SIZE+1],
             current_l_buffer [MAX_LINE_SIZE+1],
             below_l_buffer [MAX_LINE_SIZE+1];
        char *above_l = NULL, 
             *current_l = current_l_buffer, 
             *below_l = below_l_buffer;
        int line_size, sum;
        bool last_it;

        (void) fgets(current_l, MAX_LINE_SIZE, fp);
        (void) fgets(below_l, MAX_LINE_SIZE, fp);
        line_size = strlen(current_l) - 1;
        sum = 0; last_it = true;
        do {
                int number, size;
                if (below_l == NULL) last_it = false;
                for (int i = 0; i < line_size; i++) {
                        if (isdigit(current_l[i]) && 
                            check_around(current_l, above_l, below_l, line_size, i)) {
                                sscanf(current_l+i, "%d%n", &number, &size);
                                sum += number;
                                memset(current_l+i, '.', size);
                                // printf("%d\n", number);
                        }
                }

                above_l = above_l_buffer;
                strncpy(above_l, current_l, line_size);
                if (below_l != NULL) strncpy(current_l, below_l, line_size);
                if (fgets(below_l, MAX_LINE_SIZE, fp) == NULL) below_l = NULL;
        } while (below_l != NULL || last_it);
        printf("SUM=%d\n", sum);
}

static void
extend_number(char **number, int index) {
        int i;
        for (i = 0; (index - i) != -1 ;i++) {
                if  (!isdigit((*number)[-i])) break;
        }
        *number -= i - 1;
}

static int
get_gears_ratio(char *restrict cur, char *restrict above, char *restrict below, int size, int index) {
        char *pos_num1, *pos_num2, buf[10];
        int i, start, end, num1, num2;

        start = ( index==     0)? 0: -1;
        end   = ( index==size-1)? 1: 2;
        pos_num1 = NULL;
        pos_num2 = NULL;
        
        // printf("\t|%.*s|\n", end-start, above + index + start);
        // printf("\t|%.*s|\n", end-start, cur   + index + start);
        // printf("\t|%.*s|\n", end-start, below + index + start);

        for (i = start; i < end; i++) {
                if (isdigit(above[index + i])) {
                        pos_num1 = above + index + i;
                        goto found1;
                }
                if (isdigit(below[index + i])) {
                        pos_num1 = below + index + i;
                        goto found1;
                }
                if (isdigit(cur  [index + i])) {
                        pos_num1 = cur + index + i;
                        goto found1;
                }
        }
        goto failed;

found1: extend_number(&pos_num1, index+i);
        sscanf(pos_num1, "%d%n", &num1, &i);
        // printf("num1=%d\n", num1);
        
        assert(i != 0);
        
        memset(pos_num1, '.', i);

        for (i = start; i < end; i++) {
                if (isdigit(above[index + i])) {
                        pos_num2 = above + index + i;
                        goto found2;
                }
                if (isdigit(below[index + i])) {
                        pos_num2 = below + index + i;
                        goto found2;
                }
                if (isdigit(cur  [index + i])) {
                        pos_num2 = cur + index + i;
                        goto found2;
                }
        }
        goto failed;

found2: extend_number(&pos_num2, index+i);
        sscanf(pos_num2, "%d%n", &num2, &i);
        // printf("num2=%d\n", num2);

        assert(i != 0);

        memset(pos_num2, '.', i);

        for (i = start; i < end; i++) {
                if (isdigit(above[index + i])) goto failed;
                if (isdigit(below[index + i])) goto failed;
                if (isdigit(cur  [index + i])) goto failed;
        }
        sprintf(buf, "%d", num1);
        strncpy(pos_num1, buf, strlen(buf));
        sprintf(buf, "%d", num2);
        strncpy(pos_num2, buf, strlen(buf));
        return num1 * num2;

failed: sprintf(buf, "%d", num1);
        if (pos_num1) strncpy(pos_num1, buf, strlen(buf));
        sprintf(buf, "%d", num2);
        if (pos_num2) strncpy(pos_num2, buf, strlen(buf));

        // printf("\x1b[31mFailed\x1b[0m\n");
        return 0;
}

void part2(FILE *fp) {
        char above_l_buffer   [MAX_LINE_SIZE+1],
             current_l_buffer [MAX_LINE_SIZE+1],
             below_l_buffer   [MAX_LINE_SIZE+1]; 

        char *above_l = above_l_buffer,
             *cur_l   = current_l_buffer,
             *below_l = below_l_buffer;

        int len, sum;

        (void) fgets(above_l, MAX_LINE_SIZE, fp);
        (void) fgets(cur_l,   MAX_LINE_SIZE, fp);
        (void) fgets(below_l, MAX_LINE_SIZE, fp);

        len = strlen(above_l) - 1;
        sum = 0;

        do {
                for (int i = 0; i < len; i++) {
                        if (cur_l[i] == '*') {
                                sum += get_gears_ratio(cur_l, above_l, below_l, len, i);
                        }
                }
                puts(cur_l);
                strncpy(above_l, cur_l, len);
                strncpy(cur_l, below_l, len);
        } while (fgets(below_l, MAX_LINE_SIZE, fp) != NULL);
        printf("SUM Ratio=%d\n", sum);
}
