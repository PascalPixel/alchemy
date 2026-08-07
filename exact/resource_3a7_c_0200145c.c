/*
 * BYTE-EXACT and adopted 2026-08-07.  resource_3a7 carries a mirror copy of the
 * same soft-float library as resource_3bf, so this is the twin of the adopted
 * exact/resource_3bf_c_02005af0.c: identical source, with the packer renamed to
 * its direct `bl' target in this overlay (0x02002a6a).  Same recipe -- one u64
 * significand field, and the stock non-interworking ABI with r4 callee-saved so
 * the leaf can return through `pop {r4, r5, pc}'.
 * Span is 124 bytes: 112 of code plus the three pool words at 0x020014cc.
 */
#include "types.h"

/*
 * Resource 3bf signed-integer to soft-float conversion at 0x02005af0.
 *
 * Builds the library's 20-byte unpacked record on the stack and hands it to
 * the packing routine.  The record layout is fully visible here:
 *   +0  class   (2 = zero, 3 = finite)
 *   +4  sign    (r0 >> 31, i.e. 1 for a negative input)
 *   +8  exponent seed (60)
 *   +12 significand low word
 *   +16 significand high word
 *
 * Control flow:
 *   - a zero input sets class 2 and skips straight to the pack call;
 *   - the exact value 0x80000000 is special-cased and returns the literal
 *     pair r0 = 0xC1E00000, r1 = 0x00000000, i.e. -2147483648.0, without
 *     packing;  this is also the evidence that r0 carries the HIGH word of the
 *     packed double in this library, which is why the packed value is carried
 *     as SoftDouble (a u64 whose low half is the r0 word) and not as `double`;
 *   - otherwise the magnitude is placed in the significand with its sign
 *     extension in the high word and normalised by shifting the 64-bit
 *     significand left one bit at a time, decrementing the exponent, until the
 *     high word exceeds 0x0FFFFFFF.  The guard before the loop is an UNSIGNED
 *     compare (`bhi`), so a high word already above 0x0FFFFFFF skips
 *     normalisation entirely.
 *
 * Both ordinary paths join at the single local packer call at 0x02005c38.
 */
typedef u64 SoftDouble;

typedef struct SoftFloatRecord {
    u32 cls;
    u32 sign;
    s32 exponent;
    u64 fraction;
} SoftFloatRecord;

SoftDouble Func_02002a6a(SoftFloatRecord *record);

SoftDouble Func_0200145c(s32 value)
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
    return Func_02002a6a(&record);
}
