#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

#define MAX(a,b) ((a) > (b)? (a) : (b))
#define MIN(a,b) ((a) < (b)? (a) : (b))

typedef struct {
        long start, length;
        int depth;
} rangel_t; // range of longs

void rangel_print(rangel_t *r) {
        printf("{%ld, ..., %ld}\n", r->start, r->start + r->length);
}

// return the distance between start and v, -i if v is not in range
int rangel_offset(const rangel_t *r, long v) {
        return (r->start <= v && v < (r->start + r->length))? v - r->start : -1;
}

// 0 if equal
int rangel_cmp(const rangel_t *r1, const rangel_t *r2) {
        if (r1->start != r2->start) return r1->start - r2->start;
        if (r1->length!= r2->length) return r1->length - r2->length;
        return 0;
}

/* split the range in 2. start will go from r-start to v, end will go from v+1 to r->start+r->length
 * for input {50, 51, .., 100}, v = 25
 * start will be set to {50, ..., 75} and end = {76, ..., 100}
 */
void rangel_split(const rangel_t * restrict r, long v, rangel_t * restrict start, rangel_t * restrict end) {
        start->start = r->start;
        start->length = v;

        end->start = r->start + v + 1;
        end->length = r->length - v - 1;
}

/* Explode a range with relation to a subrange.
 * 2 resulting range will follow:
 * begin. {r->start, sub->start - r->start{
 * end.   {sub->start + sub->length, r->length - sub->length - begin-length}.
 * No error checking
 */
void rangel_explode(const rangel_t * restrict r, const rangel_t * restrict sub, rangel_t *start, rangel_t *end) {
        start->start = r->start;
        start->length = sub->start - r->start - 1;

        end->start = sub->start + sub->length + 1;
        end->length = r->length - sub->length - start->length - 2;
}

// 0 if invalid
int rangel_valid(const rangel_t *r) {
        static const rangel_t empty = {-1, -1, 0};
        return rangel_cmp(r, &empty);
}

// return subrange of r, of len n
rangel_t rangel_sub(const rangel_t *r, int n) {
        rangel_t res = *r;
        res.length = n;
        return res;
}

// return the intersection of r1 and r2. if the ranges do not overlap, return {-1, -1}
rangel_t range_intersect(const rangel_t *r1, const rangel_t *r2) {
        rangel_t ret;
        ret.start = MAX(r1->start, r2->start);
        ret.length = MIN(r1->start + r1->length, r2->start + r2->length) - ret.start;
        if (ret.length < 0) {
                ret.start = -1;
                ret.length = -1;
        }
        return ret;
}

int main(void) {
        rangel_t r1 = {75, 175, 0};
        rangel_t r2 = {100, 100, 0};
        rangel_t intersection = range_intersect(&r1, &r2);
        rangel_t start, end;
        
        rangel_explode(&r1, &r2, &start, &end);

        rangel_print(&r1);
        rangel_print(&r2);
        rangel_print(&intersection);
        rangel_print(&start);
        rangel_print(&end);
}
