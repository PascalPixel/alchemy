#include "types.h"
#include "scene.h"
#include "abi/map/locations/heidia/village/soft/to_int.h"

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
    Map_Apply((FloUnion *)slot, &record);

    if (Map_Do(&record) != 0u) {
        return 0;
    }
    if (Map_Do2(&record) != 0u) {
        return 0;
    }
    if (Map_Do3(&record) == 0u) {
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
    shifted = Map_Apply2(record.fraction, (u32)(60 - exponent));
    if (record.sign != 0u) {
        return -(s32)shifted;
    }
    return (s32)shifted;
}
