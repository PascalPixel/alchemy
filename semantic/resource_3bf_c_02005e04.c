#include "types.h"

/*
 * Resource 3bf soft-float double unpack at 0x02005e04.
 *
 * r0 addresses the packed pair, r1 the 20-byte record to fill.  The packed
 * pair is copied to the local sp+0/sp+4 slots purely so the exponent and sign
 * can be read back with narrower loads (`ldrh [sp,#6]`, `ldrb [sp,#7]`); the
 * halfword/byte reads confirm the library's word order, in which the FIRST
 * word of the pair is the IEEE high word carrying the sign and exponent, i.e.
 * r0 = high on entry to the packed-value ABI.  That is why the packed value is
 * carried as SoftDouble (a u64 whose low half is the first word) throughout
 * this overlay's soft-float sources rather than as a native `double`.
 *
 * Record layout, which this routine defines for the whole family:
 *   +0  class (0 signalling NaN, 1 quiet NaN, 2 zero, 3 finite, 4 infinity)
 *   +4  sign (bit 31 of the high word)
 *   +8  unbiased exponent
 *   +12 significand low word
 *   +16 significand high word, normalised so bit 28 is set
 *
 * Four cases, in the assembly's order:
 *   - biased exponent 0 with a zero significand: class 2 and nothing else;
 *   - biased exponent 0 with a non-zero significand (a denormal): exponent
 *     seeded to -1022, the 52-bit significand shifted left 8 to reach the
 *     family's 1.28-style alignment, class 3, then normalised by shifting left
 *     one bit at a time while the high word is at or below 0x0FFFFFFF (an
 *     UNSIGNED compare);
 *   - biased exponent 0x7ff: class 4 when the significand is zero, otherwise
 *     class 1 if the quiet bit (mantissa bit 51, i.e. 0x00080000 of the high
 *     mantissa field) is set and class 0 if it is not.  Both NaN classes store
 *     the significand words UNSHIFTED;
 *   - anything else: exponent minus 1023, class 3, significand shifted left 8
 *     with the implicit leading bit re-inserted as 0x10000000.
 *
 * Uncertainty: the constant 0 loaded into r1 in the normal-number path at
 * 0x02005eaa is never used before the routine returns.  It is left out here
 * rather than modelled, since it has no observable effect.
 */
typedef u64 SoftDouble;

typedef struct SoftFloatRecord {
    u32 cls;
    u32 sign;
    s32 exponent;
    u32 low;
    u32 high;
} SoftFloatRecord;

void Func_02005e04(const SoftDouble *packed, SoftFloatRecord *record)
{
    SoftDouble copy = *packed;
    u32 highWord = (u32)copy;              /* first word: sign/exponent/mantissa */
    u32 lowWord = (u32)(copy >> 32);       /* second word: mantissa low          */
    u32 mantissaHigh = highWord & 0x000fffffu;
    u32 exponent = (highWord >> 20) & 0x7ffu;

    record->sign = highWord >> 31;

    if (exponent == 0u) {
        if ((lowWord | mantissaHigh) == 0u) {
            record->cls = 2u;
            return;
        }

        record->exponent = -1022;
        mantissaHigh = (lowWord >> 24) | (mantissaHigh << 8);
        lowWord <<= 8;
        record->cls = 3u;

        while (mantissaHigh <= 0x0fffffffu) {
            mantissaHigh = (mantissaHigh << 1) | (lowWord >> 31);
            lowWord <<= 1;
            record->exponent -= 1;
        }

        record->low = lowWord;
        record->high = mantissaHigh;
        return;
    }

    if (exponent == 0x7ffu) {
        if ((lowWord | mantissaHigh) == 0u) {
            record->cls = 4u;
            return;
        }

        record->cls = (mantissaHigh & 0x00080000u) != 0u ? 1u : 0u;
        record->low = lowWord;
        record->high = mantissaHigh;
        return;
    }

    record->exponent = (s32)exponent - 1023;
    record->cls = 3u;
    record->low = lowWord << 8;
    record->high = ((lowWord >> 24) | (mantissaHigh << 8)) | 0x10000000u;
}
