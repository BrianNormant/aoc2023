#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

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

        fp = fopen(argv[1], "r");
        if (fp == NULL) {
                perror("fopen");
                exit(1);
        }
        part2(fp);
        fclose(fp);

        return 0;
}

typedef unsigned long long uint128_t;

uint128_t min_time_for_distance(uint128_t dist, uint128_t time) {
        double n,m;
        n = (double) time;
        m = (double) dist;

        return (uint128_t) ceil( (n - sqrt(n*n - 4.F*m))/2.F );
}

uint128_t max_time_for_distance(uint128_t dist, uint128_t time) {
        double n,m;
        n = (double) time;
        m = (double) dist;

        return (uint128_t) floor( (n + sqrt(n*n - 4.F*m))/2.F );
}

void part1(FILE *fp) {
        char time_line[MAX_LINE_LEN+1],
             dist_line[MAX_LINE_LEN+1];

        char *time_ptr = time_line,
             *dist_ptr = dist_line;

        assert( fgets(time_line, MAX_LINE_LEN, fp) );
        assert( fgets(dist_line, MAX_LINE_LEN, fp) );

        (void) strsep(&time_ptr, ":");
        (void) strsep(&dist_ptr, ":");

        int time, dist, nb_char_time, nb_char_dist, result;

        result = 1;
        while (sscanf(time_ptr, "%d%n", &time, &nb_char_time) == 1 &&
               sscanf(dist_ptr, "%d%n", &dist, &nb_char_dist) == 1) {
                printf("For time %d, best distance is %d\n", time, dist);
                int win_possible = max_time_for_distance(dist, time) - min_time_for_distance(dist, time) + 1;
;
                // for (int i = 1; i < time; i++) {
                //         int travel = i * (time - i);
                //         win_possible += (travel > dist)?1:0;
                // }

                result *= win_possible;
                time_ptr += nb_char_time;
                dist_ptr += nb_char_dist;
        }
        printf("Result is %d\n", result);
}


void part2(FILE *fp) {
        char time_line[MAX_LINE_LEN+1],
             dist_line[MAX_LINE_LEN+1];

        char *time_ptr = time_line,
             *dist_ptr = dist_line,
             *space_ptr;
        
        assert( fgets(time_line, MAX_LINE_LEN, fp) );
        assert( fgets(dist_line, MAX_LINE_LEN, fp) );

        (void) strsep(&time_ptr, ":");
        (void) strsep(&dist_ptr, ":");

        while ((space_ptr = strstr(time_ptr, " "))) {
                int len = strlen(space_ptr+1);
                (void) memmove(space_ptr, space_ptr + 1, len+1);
        }

        while ((space_ptr = strstr(dist_ptr, " "))) {
                int len = strlen(space_ptr+1);
                (void) memmove(space_ptr, space_ptr + 1, len+1);
        }

        printf(time_ptr);
        printf(dist_ptr);

        uint128_t time, dist;

        sscanf(time_ptr, "%llu", &time);
        sscanf(dist_ptr, "%llu", &dist);

        uint128_t result = max_time_for_distance(dist, time) - min_time_for_distance(dist, time) + 1;

        printf("Result is %llu\n", result);
}
