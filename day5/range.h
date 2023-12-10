// A set in R, with function associated to make subset, intersection, ect
#include "cc.h"
typedef struct {
        long start, length;
} rangel_t; // range of longs
#define CC_CMPR rangel_t, { return (val_1.start < val_2.start)? -1 : val_1.start > val_2.start; }
#define CC_HASH rangel_t, { \
        long r = val.start % val.length * 65599; \
        return r; \
}

#include "cc.h"

extern const rangel_t RANGEL_INVALID;

extern int inline rangel_get_end(const rangel_t *r);

extern void rangel_print(const rangel_t *r);

extern int rangel_offset(const rangel_t *r, long v);

extern int rangel_cmp(const rangel_t *r1, const rangel_t *r2);

extern void rangel_split(const rangel_t *  r, long v, rangel_t * restrict start, rangel_t * restrict end);

extern int rangel_explode(const rangel_t * restrict r, const rangel_t * restrict sub, rangel_t *start, rangel_t *end);

extern int rangel_valid(const rangel_t *r);

extern int rangel_overlap(const rangel_t *r1, const rangel_t *r2);

extern rangel_t rangel_sub(const rangel_t *r, long from, long to);

extern rangel_t rangel_intersect(const rangel_t *r1, const rangel_t *r2);
