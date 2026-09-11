#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/state/interaction/soft_double_to_signed_int.h"

typedef u64 SoftDouble;

typedef union FloUnion {
    SoftDouble value;
    struct {
        u32 lo;
        u32 hi;
    } words;
} FloUnion;

typedef struct SoftFloatRecord {
    u32 cls;
    u32 sign;
    s32 exp;
    u64 fraction;
} SoftFloatRecord;

s32 Runtime_SoftDoubleToSignedInt(u32 high, u32 low)
{
    SoftFloatRecord rec;
    volatile FloUnion u;
    volatile FloUnion *p;
    s32 exp;
    u64 frac;

    p = &u;
    p->words.lo = high;
    p->words.hi = low;
    State_Apply((FloUnion *)p, &rec);

    if (State_Do(&rec) != 0u) {
        return 0;
    }
    if (State_Do2(&rec) != 0u) {
        return 0;
    }
    if (State_Do3(&rec) == 0u) {
        exp = rec.exp;
        if (exp < 0) {
            return 0;
        }
        if (exp <= 30) {
            goto convert;
        }
    }
    return 0x7fffffff + (s32)(rec.sign != 0u);

convert:
    frac = State_Apply2(rec.fraction, (u32)(60 - exp));
    if (rec.sign != 0u) {
        return -(s32)frac;
    }
    return (s32)frac;
}
