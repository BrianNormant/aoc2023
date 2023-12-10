#include <stdio.h>
#include "range.h"
#include <assert.h>

#define MAX(a,b) ((a) > (b)? (a) : (b))
#define MIN(a,b) ((a) < (b)? (a) : (b))

void rangel_print(const rangel_t *r) {
        if (!rangel_valid(r)) {
                printf("{INVALID}");
        } else {
                printf("{%ld, ..., %ld}", r->start, r->start + r->length);
        }
}

int inline rangel_get_end(const rangel_t *r) {
        return r->start + r->length - 1;
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
 * end.   }sub->start + sub->length, r->length - sub->length - begin-length}.
 * return 1 if the only begin exist, 2 if the only end exist, 3 if both, 0 otherwise.
 */
int rangel_explode(const rangel_t * restrict r, const rangel_t * restrict sub, rangel_t *start, rangel_t *end) {
        int res = 0;

        if (sub->start <= r->start) {
                *start = RANGEL_INVALID;
        } else {
                start->start = r->start;
                start->length = sub->start - r->start - 1;
                res |= 1; // Set the first bit to indicate the presence of the start range
        }

        if (rangel_get_end(sub) >= rangel_get_end(r)) {
                *end = RANGEL_INVALID;
        } else {
                end->start = rangel_get_end(sub) + 2;
                end->length = rangel_get_end(r) - rangel_get_end(sub) - 1;
                res |= 2; // Set the second bit to indicate the presence of the end range
        }

        return res;
}

const rangel_t RANGEL_INVALID = {-1, -1};

// 0 if invalid
int rangel_valid(const rangel_t *r) {
        return rangel_cmp(r, &RANGEL_INVALID);
}

// return a subrange from r->start + from to r->start + to - 1
// from and to are relative to r->start;
rangel_t rangel_sub(const rangel_t *r, long from, long to) {
        rangel_t res;
        assert(to >= from);

        res.start = r->start + from;
        res.length = r->start + to - res.start;

        return res;
}

// return the intersection of r1 and r2. if the ranges do not overlap, return {-1, -1}
rangel_t rangel_intersect(const rangel_t *r1, const rangel_t *r2) {
        rangel_t ret;
        if (rangel_get_end(r1) < r2->start || rangel_get_end(r2) < r1->start) {
                return RANGEL_INVALID;
        }
        ret.start = MAX(r1->start, r2->start);
        ret.length = MIN(r1->start + r1->length, r2->start + r2->length) - ret.start;
        return ret;
}


int rangel_overlap(const rangel_t *r1, const rangel_t *r2) {
        rangel_t tmp = rangel_intersect(r1, r2);
        return rangel_valid(&tmp);
}

#undef MIN
#undef MAX

// int main(void) {
//         rangel_t r1 = {55, 14, 0};
//         rangel_t r2 = {52, 50, 0};
//
//         rangel_t r3 = rangel_intersect(&r1, &r2);
//         r3 = rangel_sub(&r1, 4, 10);
//         
//         rangel_print(&r1);
//         rangel_print(&r3);
// }
