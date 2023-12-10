#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LEN 1000

void part1(FILE *fp);
void part2(FILE *fp);

int main(int argc, char *argv[]) {
        if (argc!= 2) {
                fprintf(stderr, "Usage: %s <input>\n", argv[0]);
                exit(1);
        }
        FILE *fp;

        fp = fopen(argv[1], "r");
        if (fp == NULL) {
                perror("fopen");
                exit(1);
        }
        part1(fp);
        fclose(fp);


        // fp = fopen(argv[1], "r");
        // if (fp == NULL) {
        //         perror("fopen");
        //         exit(1);
        // }
        // part2(fp);
        // fclose(fp);

        return 0;
}


typedef struct node_s node_t;

typedef union {
    char name[3];
    node_t *ptr;
} child_t;

typedef struct node_s {
    char name[3];
    child_t right;
    child_t left;
} node_t;

void link_all(node_t nodes[], int node_size) {
        for (int i = 0; i < node_size; i++) {
                char *parent = nodes[i].name;
                for (int j = 0; j < node_size; j++) {
                }
        }
}

void part1(FILE *fp) {
        char line_buf[MAX_LINE_LEN],
            *line;


}
