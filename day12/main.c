#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

        int solution = 0;

        while (fgets(line, MAX_LINE_LEN, fp)!= NULL) {
                line = line_buf;
        }

        printf("Solution: %d\n", solution);
}
void part2(FILE *fp) {}
