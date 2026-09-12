/*
 * Signed integer to soft-float conversion for resource_3bf.  The 124-byte
 * owner is 112 bytes of code plus its three pool words.  It is a leaf on
 * the non-interworking ABI, with r4 callee-saved.
 */
#include "types.h"

/*
 * The 20-byte unpacked record the library packs from:
 *   +0  class (2 = zero, 3 = finite)
 *   +4  sign (1 for a negative input)
 *   +8  exponent seed (60)
 *   +12 significand, 64 bits
 *
 * The significand must stay one 64-bit field.  Split into two words, the
 * normalising loop keeps the pair in registers instead of loading and
 * storing it through the record on every iteration.  r0 carries the high
 * word of the packed double, so the packed value is a u64 and not a double.
 */
typedef u64 SoftDouble;

typedef struct SoftFloatRecord {
    u32 cls;
    u32 sign;
    s32 exponent;
    u64 fraction;
} SoftFloatRecord;

/* The packer, reached by a direct call rather than through a veneer. */
SoftDouble Func_0200b792(SoftFloatRecord *record);

/*
 * A zero input takes class 2 straight to the pack call, and 0x80000000
 * returns its packed value directly.  The guard before the normalising loop
 * is an unsigned compare, so a significand already above 0x0fffffffffffffff
 * skips normalisation entirely.
 */
SoftDouble SignedIntToSoftDouble(s32 value)
{
    SoftFloatRecord record;
    u32 sign;

    record.cls = 3u;
    sign = (u32)value >> 31;
    record.sign = sign;

    if (value == 0) {
        record.cls = 2u;
        goto pack;
    }

    record.exponent = 60;

    if (sign != 0u) {
        if ((u32)value == 0x80000000u) {
            return (SoftDouble)0xC1E00000u;
        }
        record.fraction = (u64)(s64)(-value);
    } else {
        record.fraction = (u64)(s64)value;
    }

    while (record.fraction <= 0x0FFFFFFFFFFFFFFFu) {
        record.fraction = record.fraction << 1;
        record.exponent -= 1;
    }

pack:
    return Func_0200b792(&record);
}
