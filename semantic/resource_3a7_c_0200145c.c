#include "types.h"

/*
 * resource_3a7 signed-integer to soft-float conversion at 0x0200145c, 124 bytes.
 *
 * TRANSPOSED from semantic/overlays/resource_3bf_c_02005af0.c.  The two owners
 * are the same routine shared verbatim: over all 62 halfwords they differ in
 * exactly 2 places, both halves of the single BL pair.  No pool word differs.
 *
 * What was changed:
 *  - the entry symbol;
 *  - the one call, re-resolved with 'bun tools/overlay_call_targets.ts
 *    resource_3a7 145c': a single site whose target is this overlay's own
 *    prologue at 0x020015a4 (in resource_3bf the same site targets 0x02005c38).
 *    The 3bf source predates the corrected 'bl' rule and named it Func_0200b792
 *    while noting it as "a resident service whose address falls numerically
 *    inside this overlay's image"; under the corrected rule it is simply the
 *    overlay's own soft-float packing routine, and it is named accordingly here.
 *
 * Builds the library's 20-byte unpacked record on the stack and hands it to the
 * packing routine:
 *   +0 class (2 = zero, 3 = finite), +4 sign, +8 exponent seed (60),
 *   +12 significand low word, +16 significand high word.
 *
 * A zero input sets class 2 and skips to the pack call; the exact value
 * 0x80000000 is special-cased and returns the literal pair r0 = 0xC1E00000,
 * r1 = 0 (which is also the evidence that r0 carries the HIGH word of the packed
 * double); otherwise the magnitude is normalised by shifting the 64-bit
 * significand left until the high word exceeds 0x0FFFFFFF, an UNSIGNED compare
 * ('bhi'), so a high word already above that skips normalisation entirely.
 */
typedef u64 SoftDouble;

typedef struct SoftFloatRecord {
    u32 cls;
    u32 sign;
    s32 exponent;
    u32 low;
    u32 high;
} SoftFloatRecord;

SoftDouble Func_020015a4(SoftFloatRecord *record);

SoftDouble Func_0200145c(s32 value)
{
    SoftFloatRecord record;
    u32 sign = (u32)value >> 31;

    record.cls = 3u;
    record.sign = sign;

    if (value == 0) {
        record.cls = 2u;
        /* `b.n 0x020014c2` -- the zero case reaches the SAME pack call site at
         * 0x020014c4 as the normal path; there is one call, not two. */
        goto pack;
    }

    record.exponent = 60;

    if (sign != 0u) {
        if ((u32)value == 0x80000000u) {
            /* r0 = 0xC1E00000, r1 = 0x00000000 */
            return (SoftDouble)0xC1E00000u;
        }
        record.low = (u32)(-value);
        record.high = (u32)((-value) >> 31);
    } else {
        record.low = (u32)value;
        record.high = (u32)(value >> 31);
    }

    if (record.high <= 0x0FFFFFFFu) {
        do {
            u32 low = record.low;
            u32 high = record.high;

            record.low = low << 1;
            record.high = (high << 1) | (low >> 31);
            record.exponent -= 1;
        } while (record.high <= 0x0FFFFFFFu);
    }

pack:
    return Func_020015a4(&record);
}
