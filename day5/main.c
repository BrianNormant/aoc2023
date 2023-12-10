#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <assert.h>

#include "cc.h"
#include "range.h"

#define MAX_LINE 1000
#define MAX(a,b) ((a) > (b)? (a) : (b))
#define MIN(a,b) ((a) < (b)? (a) : (b))

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

typedef struct {
        long intermediate;
        long final;
} seed_map_t;

void part1(FILE *fp) {
        map( long, seed_map_t ) seed_map;
        char line_buf[MAX_LINE+1],
            *line = line_buf,
            *tok;

        init(&seed_map);
        while (fgets(line, MAX_LINE, fp) != NULL) {
                long dst_r, src_r, size_r;
                if (!strncmp(line, "seeds:", 6)) {
                        line += 7;
                        while ((tok = strsep(&line, " ")) != NULL) {
                                if (tok[0] == '\n') break;
                                long seed;
                                seed_map_t m;

                                seed = atol(tok);
                                m.intermediate = seed;
                                insert(&seed_map, seed, m);
                        }
                } else if (strstr(line, "map")) {
                        for_each(&seed_map, seed, map) {
                                map->final = map->intermediate;
                                map->intermediate = -1;
                        }
                } else if (line[0] == '\n') {
                        for_each(&seed_map, seed, map) {
                                map->intermediate = (map->intermediate == -1)?
                                        map->final : map->intermediate;
                        }
                } else if (sscanf(line, "%ld %ld %ld", &dst_r, &src_r, &size_r) == 3) {
                        for_each(&seed_map, seed, map) {
                                int off = (map->final >= src_r && map->final < (src_r + size_r))? 
                                        map->final - src_r : -1;
                                if (off != -1)
                                        map->intermediate = dst_r + off;
                        }
                }
                line = line_buf;
        }
        
        seed_map_t min = {INT_MAX, INT_MAX};
        for_each(&seed_map, key, map) {
                printf("%ld -> %ld\n", *key, map->intermediate);
                if (map->intermediate < min.intermediate) min = *map;
        }

        printf("Min location is %ld\n", min.intermediate);
        cleanup(&seed_map);
}

int ranges_unique(map( rangel_t, rangel_t ) ranges) {
        vec(rangel_t) v, d;
        int res = 1;
        long total_seed = 0;

        init(&v);
        init(&d);
        for_each(&ranges, src, dst) {
                total_seed += src->length;
                for_each(&v, i) {
                        if (rangel_overlap(src, i)) {
                                res = 0;
                                printf("\x1b[32m src ranges overlap:");
                                rangel_print(src);
                                rangel_print(i);
                                rangel_t inters = rangel_intersect(src, i);
                                rangel_print(&inters);
                                printf("\x1b[0m\n");
                                goto failure;
                        }
                }
                for_each(&d, i) {
                        if (rangel_overlap(dst, i)) {
                                res = 0;
                                printf("\x1b[33m dst ranges overlap:");
                                rangel_print(dst);
                                rangel_print(i);
                                rangel_t inters = rangel_intersect(dst, i);
                                rangel_print(&inters);
                                printf("\x1b[0m\n");
                                goto failure;
                        }
                }
                push(&v, *src);
                push(&d, *dst);
        }
 failure:printf("\x1b[34mTotal seed: %ld\x1b[0m\n", total_seed);
         cleanup(&v);
         cleanup(&d);
         return res;
}

void part2(FILE *fp) {
        map( long, seed_map_t ) seed_map;
        char line_buf[MAX_LINE+1],
            *line = line_buf,
            *tok;

        init(&seed_map);
        while (fgets(line, MAX_LINE, fp) != NULL) {
                printf("%s", line);
                long dst_r, src_r, size_r;
                if (!strncmp(line, "seeds:", 6)) {
                        line += 7;
                        long start, length;
                        int size;
                        seed_map_t m;
                        while (sscanf(line, "%ld %ld%n", &start, &length, &size) == 2) {
                                printf("Adding seed %ld to %ld\n", start, start+length);
                                for (long i = 0; i < length; i++) {
                                        m.intermediate = start+i;
                                        insert(&seed_map, start+i, m);

                                }
                                line += size;
                        }
                } else if (strstr(line, "map")) {
                        for_each(&seed_map, seed, map) {
                                map->final = map->intermediate;
                                map->intermediate = -1;
                        }
                } else if (line[0] == '\n') {
                        for_each(&seed_map, seed, map) {
                                map->intermediate = (map->intermediate == -1)?
                                        map->final : map->intermediate;
                        }
                } else if (sscanf(line, "%ld %ld %ld", &dst_r, &src_r, &size_r) == 3) {
                        for_each(&seed_map, seed, map) {
                                int off = (map->final >= src_r && map->final < (src_r + size_r))? 
                                        map->final - src_r : -1;
                                if (off != -1)
                                        map->intermediate = dst_r + off;
                        }
                }
                line = line_buf;
        }
        
        seed_map_t min = {LONG_MAX, LONG_MAX};
        for_each(&seed_map, key, map) {
                printf("%ld -> %ld\n", *key, map->intermediate);
                if (map->intermediate < min.intermediate) min = *map;
        }

        printf("Min location is %ld\n", min.intermediate);
        cleanup(&seed_map);
}

void part2f(FILE *fp) {
        map( rangel_t, rangel_t ) range_map, range_map_tmp;
        char line_buf[MAX_LINE+1],
            *line = line_buf;
        
        init(&range_map);
        while (fgets(line, MAX_LINE, fp) != NULL) {
                long dst_r, src_r, size_r;
                printf("%s", line);
                if (!strncmp(line, "seeds:", 6)) {
                        line += 7;
                        
                        int len; rangel_t r;
                        while (sscanf(line, "%ld %ld%n", &dst_r, &size_r, &len) == 2) {
                                r.start = dst_r; 
                                r.length = size_r-1; 
                                assert(insert(&range_map, r, r));
                                line += len;
                        }
                } else if (strstr(line, "map")) {
                        rangel_t r3 = {-1, -1};
                        
                        init(&range_map_tmp);
                        for_each(&range_map, r1, r2) {
                                assert(insert(&range_map_tmp, *r2, r3));
                        }
                        cleanup(&range_map);

                        range_map = range_map_tmp;
                } else if (sscanf(line, "%ld %ld %ld", &dst_r, &src_r, &size_r) == 3) {
                        init(&range_map_tmp);

                        for_each(&range_map, r1, r2) {
                                rangel_t destination = {dst_r, size_r-1}, 
                                         source      = {src_r, size_r-1}, 
                                         intersection, start, end;

                                start = end = RANGEL_INVALID;
                                intersection = rangel_intersect(r1, &source);
                                if (!rangel_valid(&intersection)) {
                                        assert(insert(&range_map_tmp, *r1, *r2));
                                } else if (!rangel_cmp(&intersection, r1)) {
                                        destination = rangel_sub(&destination, intersection.start - source.start, intersection.start - source.start + intersection.length);
                                        assert(insert(&range_map_tmp, *r1, destination));
                                        printf("\x1b[31m source is bigger than intersection\x1b[0m\n");
                                } else {
                                        switch(rangel_explode(r1, &intersection, &start, &end)) {
                                        case 1: destination = rangel_sub(&destination, 0, intersection.length);
                                                assert(insert(&range_map_tmp, start, RANGEL_INVALID));
                                                assert(insert(&range_map_tmp, intersection, destination));
                                                printf("\x1b[31mExplode 1\x1b[0m\n");
                                                break;
                                        case 2: destination = rangel_sub(&destination, intersection.start - source.start, intersection.start - source.start + intersection.length);
                                                assert(insert(&range_map_tmp, end, RANGEL_INVALID));
                                                assert(insert(&range_map_tmp, intersection, destination));
                                                printf("\x1b[31mExplode 2\x1b[0m\n");
                                                assert(rangel_valid(&destination));
                                                break;
                                        case 3: assert(insert(&range_map_tmp, intersection, destination));
                                                assert(insert(&range_map_tmp, start, RANGEL_INVALID));
                                                assert(insert(&range_map_tmp, end, RANGEL_INVALID));
                                                printf("\x1b[31mExplode 3\x1b[0m\n");
                                                break;
                                        default: perror("explode_unexpected"); 
                                                 exit(1);
                                        }
                                }

                        }
                        cleanup(&range_map);
                        range_map = range_map_tmp;
                } else {
                        printf("\x1b[31mRanges Flip\x1b[0m\n");
                
                        init(&range_map_tmp);
                        for_each(&range_map, r1, r2) {
                                if (!rangel_valid(r2)) 
                                        insert(&range_map_tmp, *r1, *r1);
                                else insert(&range_map_tmp, *r1, *r2);
                        }
                        cleanup(&range_map);
                        range_map = range_map_tmp;
                }
                assert(ranges_unique(range_map));
                line = line_buf;
        }

        long min = LONG_MAX;
        for_each(&range_map, src, dst) {
                rangel_print(src);
                printf(" -> ");
                rangel_print(dst);
                printf("\n");
                if (dst->start < min) min = dst->start;
        }

        printf("Min location is %ld\n", min);
        cleanup(&range_map);
}
