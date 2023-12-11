#include <stdio.h>
#include <stdlib.h>

#define FILE_NAME "example.txt"
#define MAX_LINE_LEN 1000

void part1(FILE *fp);
void part2(FILE *fp);

int main(int argc, char *argv[]) {
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


void part1(FILE *fp) {}
void part2(FILE *fp) {}
