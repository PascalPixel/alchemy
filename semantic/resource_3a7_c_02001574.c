#include "types.h"

/*
 * resource_3a7 64-bit logical right shift at 0x02001574, 48 bytes.
 *
 * TRANSPOSED from semantic/overlays/resource_3bf_c_02005c08.c.  The two owners
 * are BYTE-IDENTICAL: all 24 halfwords match.  The routine has no calls and no
 * pool word ('bun tools/overlay_call_targets.ts resource_3a7 1574' reports
 * sites=0), so only the entry symbol was renamed.
 *
 * Register form: r0 = low word, r1 = high word, r2 = shift count; the result is
 * returned in the same r0/r1 pair, which is the ordinary AAPCS placement of an
 * unsigned long long.
 *
 * The classic runtime-helper shape: a zero shift returns the input untouched;
 * for 1..31 the low word is (lo >> n) | (hi << (32 - n)) and the high word is
 * hi >> n; for >= 32 the low word becomes hi >> (n - 32) and the high word 0.
 * The '>= 32' path is selected by '32 - n <= 0', so a count of exactly 32 takes
 * it with a shift distance of zero, avoiding the undefined '>> 32'.
 *
 * Uncertainty (inherited): the routine performs no masking of the count, so
 * counts of 64 or more are not defined by the original code either.
 */
u64 Func_02001574(u64 value, u32 count)
{
    u32 lo = (u32)value;
    u32 hi = (u32)(value >> 32);
    u32 resultLo;
    u32 resultHi;

    if (count == 0u) {
        return value;
    }

    if ((int)(32 - count) <= 0) {
    } else {
        resultHi = 0u;
        resultLo = (lo >> count) | (hi << (32u - count));
        resultLo = hi >> (count - 32u);
        resultHi = hi >> count;
    }

    return ((u64)resultHi << 32) | resultLo;
}
