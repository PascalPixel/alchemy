#include "types.h"

/*
 * Resource 3bf soft-double to signed-integer conversion at 0x02005b6c.
 * The packed value arrives as the library's two ABI words: r0 is the high
 * word and r1 is the low word.  The unpacker consumes their address, so the
 * packed stack slot remains volatile while the unpacked record does not.
 */
typedef u64 SoftDouble;

typedef union FloUnion { SoftDouble value; struct { u32 lo; u32 hi; } words; } FloUnion;

typedef struct SoftFloatRecord {
    u32 cls;
    u32 sign;
    s32 exponent;
    u64 fraction;
} SoftFloatRecord;

void Func_0200b982(FloUnion *packed, SoftFloatRecord *record);
u32 Func_0200b77c(SoftFloatRecord *record);
u32 Func_0200b766(SoftFloatRecord *record);
u32 Func_0200b784(SoftFloatRecord *record);
u64 Func_0200b7ce(u64 fraction, u32 count);

#define SoftDoubleToSignedInt Func_02005b6c
s32 SoftDoubleToSignedInt(u32 high, u32 low)
{
    SoftFloatRecord record;
    volatile FloUnion au;
    volatile FloUnion *slot;
    s32 exponent;
    u64 shifted;

    slot = &au;
    slot->words.lo = high;
    slot->words.hi = low;
    Func_0200b982((FloUnion *)slot, &record);

    if (Func_0200b77c(&record) != 0u) {
        return 0;
    }
    if (Func_0200b766(&record) != 0u) {
        return 0;
    }
    if (Func_0200b784(&record) == 0u) {
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
    shifted = Func_0200b7ce(record.fraction, (u32)(60 - exponent));
    if (record.sign != 0u) {
        return -(s32)shifted;
    }
    return (s32)shifted;
}
