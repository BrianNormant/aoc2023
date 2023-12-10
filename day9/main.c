#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void part1(FILE *fp);
void part2(FILE *fp);

#define FILE_NAME "puzzle.txt"
#define MAX_LINE_LEN 1000
#define MAX_ELEMENTS 100

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

void intarr_print(int *arr, int size) {
        int i;
        printf("[");
        for (i = 0; i < size; i++) {
                printf("%3d, ", arr[i]);
        }
        printf("]\n");
}

int intarr_allY(int *arr, int size, int y) {
        int i;
        for (i = 0; i < size; i++) {
                if (arr[i] != y) return 0;
        }
        return 1;
}

void part1(FILE *fp) {
        char line_buf[MAX_LINE_LEN];
        char*line = line_buf;
        int elements[MAX_ELEMENTS*MAX_ELEMENTS];
        int sum = 0;

        while (fgets(line, MAX_LINE_LEN, fp) != NULL) {
                char *tok;
                int el_size, step, last;
                
                el_size = 0;
                while ((tok = strsep(&line, " ")) != NULL) {
                        if (tok[0] == '\n') break;
                        int n = atoi(tok);
                        assert(el_size < MAX_ELEMENTS - 1);
                        elements[el_size++] = n;
                }
                line = line_buf;


                intarr_print(elements, el_size);

                step = 0;
                do {
                        step++;

                        for (int i = 0; i < el_size - step; i++) {
                                elements[(step*MAX_ELEMENTS) + i] = 
                                        elements[((step-1)*MAX_ELEMENTS) + i + 1] - 
                                        elements[((step-1)*MAX_ELEMENTS) + i];
                        }

                        assert(step < MAX_ELEMENTS);
                } while (!intarr_allY(elements + step*MAX_ELEMENTS, el_size - step, 0));

                elements[step*MAX_ELEMENTS + el_size - step] = 0;


                for (int i = step - 1; i >= 0; i--) {
                        last = elements[i*MAX_ELEMENTS + el_size - i] = 
                                elements[i*MAX_ELEMENTS + el_size - i - 1] + 
                                elements[(i+1)*MAX_ELEMENTS + el_size - i - 1];
                        // intarr_print(elements + i*MAX_ELEMENTS, el_size - i + 1);
                }
                sum += last;
        }
        printf("Sum of extrapolated values = %3d\n", sum);
}

void part2(FILE *fp) {
        char line_buf[MAX_LINE_LEN];
        char*line = line_buf;
        int elements_buf[MAX_ELEMENTS*MAX_ELEMENTS + 1];
        int *elements = elements_buf + 1;
        int sum = 0;

        while (fgets(line, MAX_LINE_LEN, fp) != NULL) {
                char *tok;
                int el_size, step, last;
                
                el_size = 0;
                (void) memset(elements, 0, MAX_ELEMENTS*MAX_ELEMENTS*sizeof(int));
                while ((tok = strsep(&line, " ")) != NULL) {
                        if (tok[0] == '\n') break;
                        int n = atoi(tok);
                        assert(el_size < MAX_ELEMENTS - 1);
                        elements[el_size++] = n;
                }
                line = line_buf;



                

                intarr_print(elements, el_size);

                step = 0;
                do {
                        step++;

                        for (int i = step; i < el_size; i++) {
                                elements[step*MAX_ELEMENTS + i] = 
                                        elements[((step-1)*MAX_ELEMENTS) + i] - 
                                        elements[((step-1)*MAX_ELEMENTS) + i - 1];
                        }

                        // intarr_print(elements + step*MAX_ELEMENTS + step, el_size - step);

                        assert(step < MAX_ELEMENTS);
                } while (!intarr_allY(elements + step*MAX_ELEMENTS + step, el_size - step, 0));

                elements[step*MAX_ELEMENTS + step - 1] = 0;
                
                for (int i = step - 1; i >= 0; i--) {
                        last = elements[i*MAX_ELEMENTS + i - 1] =
                               elements[i*MAX_ELEMENTS + i] - 
                               elements[(i+1)*MAX_ELEMENTS + i];
                        // intarr_print(elements + i*MAX_ELEMENTS + i - 1, el_size - i + 1);
                }
                sum += last;
        }
        printf("Sum of extrapolated values = %d\n", sum);
}
