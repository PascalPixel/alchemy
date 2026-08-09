#include "types.h"

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
    s32 exponent;
    u64 fraction;
} SoftFloatRecord;

void Func_02002c5a(FloUnion *packed, SoftFloatRecord *record);
u32 Func_02002a54(SoftFloatRecord *record);
u32 Func_02002a3e(SoftFloatRecord *record);
u32 Func_02002a5c(SoftFloatRecord *record);
u64 Func_02002aa6(u64 fraction, u32 count);

s32 Func_020014d8(u32 high, u32 low)
{
    SoftFloatRecord record;
    volatile FloUnion au;
    volatile FloUnion *slot;
    s32 exponent;
    u64 shifted;

    slot = &au;
    slot->words.lo = high;
    slot->words.hi = low;
    Func_02002c5a((FloUnion *)slot, &record);

    if (Func_02002a54(&record) != 0u) {
        return 0;
    }
    if (Func_02002a3e(&record) != 0u) {
        return 0;
    }
    if (Func_02002a5c(&record) == 0u) {
        exponent = record.exponent;
        if (exponent < 0) {
            return 0;
        }
        if (exponent <= 30) {
            goto convert;
        }
    }
    return 0x7fffffff + (s32)(record.sign != 0u);

convert:
    shifted = Func_02002aa6(record.fraction, (u32)(60 - exponent));
    if (record.sign != 0u) {
        return -(s32)shifted;
    }
    return (s32)shifted;
}
