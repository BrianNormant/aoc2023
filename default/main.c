#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

// #define FILE_NAME "puzzle.txt"
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


void part1(FILE *fp) {
        char line_buf[MAX_LINE_LEN];
        char *line = line_buf;


        while (fgets(line, MAX_LINE_LEN, fp) != NULL) {
                line = line_buf;
        }

        int solution = 0;
        printf("Solution 1: %d\n", solution);
}
void part2(FILE *fp) {
        char line_buf[MAX_LINE_LEN];
        char *line = line_buf;


        while (fgets(line, MAX_LINE_LEN, fp) != NULL) {
                line = line_buf;
        }

        int solution = 0;
        printf("Solution 2: %d\n", solution);
}
