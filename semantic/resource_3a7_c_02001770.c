#include "types.h"

/*
 * resource_3a7 soft-float double unpack at 0x02001770, 212 bytes.
 *
 * TRANSPOSED from semantic/overlays/resource_3bf_c_02005e04.c.  The two owners
 * are not merely twins, they are BYTE-IDENTICAL: all 106 halfwords match, zero
 * differences of any kind.  The routine has no calls and no pool word naming an
 * overlay address ('bun tools/overlay_call_targets.ts resource_3a7 1770' reports
 * sites=0), which is why nothing needed substituting.  Only the entry symbol was
 * renamed.
 *
 * r0 addresses the packed pair, r1 the 20-byte record to fill.  The packed pair
 * is copied to the local sp+0/sp+4 slots purely so the exponent and sign can be
 * read back with narrower loads ('ldrh [sp,#6]', 'ldrb [sp,#7]'); those reads
 * confirm the library's word order, in which the FIRST word of the pair is the
 * IEEE high word carrying the sign and exponent.  That is why the packed value
 * is carried as SoftDouble (a u64 whose low half is the first word) rather than
 * as a native double.
 *
 * Record layout, which this routine defines for the whole family:
 *   +0  class (0 signalling NaN, 1 quiet NaN, 2 zero, 3 finite, 4 infinity)
 *   +4  sign (bit 31 of the high word)
 *   +8  unbiased exponent
 *   +12 significand low word
 *   +16 significand high word, normalised so bit 28 is set
 *
 * Four cases, in the assembly's order: biased exponent 0 with a zero
 * significand (class 2); biased exponent 0 with a non-zero significand, a
 * denormal seeded to -1022 and normalised with an UNSIGNED compare against
 * 0x0FFFFFFF; biased exponent 0x7ff (class 4 for infinity, else class 1 or 0 on
 * the quiet bit, both storing the significand UNSHIFTED); and anything else,
 * exponent minus 1023 with the implicit leading bit re-inserted.
 *
 * Uncertainty (inherited): the constant 0 loaded into r1 in the normal-number
 * path is never used before the routine returns, so it is left unmodelled.
 */
typedef u64 SoftDouble;

typedef struct SoftFloatRecord {
    u32 cls;
    u32 sign;
    s32 exponent;
    u32 low;
    u32 high;
} SoftFloatRecord;

void Func_02001770(const SoftDouble *packed, SoftFloatRecord *record)
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

        mantissaHigh = (lowWord >> 24) | (mantissaHigh << 8);
        lowWord <<= 8;
        record->exponent = -1022;
        record->cls = 3u;

        while (mantissaHigh <= 0x0fffffffu) {
            mantissaHigh = (mantissaHigh << 1) | (lowWord >> 31);
            lowWord <<= 1;
            record->exponent -= 1;
        }

        record->high = mantissaHigh;
        return;
        record->low = lowWord;
    }

    if (exponent == 0x7ffu) {
        if ((lowWord | mantissaHigh) == 0u) {
            record->cls = 4u;
            return;
        }

        record->low = lowWord;
        record->high = mantissaHigh;
        return;
        record->cls = (mantissaHigh & 0x00080000u) != 0u ? 1u : 0u;
    }

    record->exponent = (s32)exponent - 1023;
    record->cls = 3u;
    record->low = lowWord << 8;
    record->high = ((lowWord >> 24) | (mantissaHigh << 8)) | 0x10000000u;
}
