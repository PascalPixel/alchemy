#include "types.h"

#define Runtime_SoftDoubleToSignedInt Func_020014d8

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

void Func_02002c5a(FloUnion *packed, SoftFloatRecord *rec);
u32 Func_02002a54(SoftFloatRecord *rec);
u32 Func_02002a3e(SoftFloatRecord *rec);
u32 Func_02002a5c(SoftFloatRecord *rec);
u64 Func_02002aa6(u64 fraction, u32 count);

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
    Func_02002c5a((FloUnion *)p, &rec);

    if (Func_02002a54(&rec) != 0u) {
        return 0;
    }
    if (Func_02002a3e(&rec) != 0u) {
        return 0;
    }
    if (Func_02002a5c(&rec) == 0u) {
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
    frac = Func_02002aa6(rec.fraction, (u32)(60 - exp));
    if (rec.sign != 0u) {
        return -(s32)frac;
    }
    return (s32)frac;
}
