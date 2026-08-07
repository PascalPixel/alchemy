/*
 * BYTE-EXACT and adopted 2026-08-07.  Two things closed it.  The body had to be
 * written with the else arm in reference order -- carry, shifted low, high,
 * then the OR -- with the `>= 32' arm setting the high half to zero before the
 * low half.  And the epilogue: this is another resource_3bf soft-library leaf
 * returning through `pop {r4,r5,r6,pc}', so it needs the stock non-interworking
 * ABI with r4 callee-saved, like its 02005a40 and 02005a78 siblings.
 */
#include "types.h"

/*
 * Resource 3bf 64-bit logical right shift at 0x02005c08.
 *
 * Register form: r0 = low word, r1 = high word, r2 = shift count; the result
 * is returned in the same r0/r1 pair.  That is the ordinary AAPCS placement of
 * a `unsigned long long` argument pair and return value, so it is spelled that
 * way here.
 *
 * The assembly reproduces the classic runtime-helper shape:
 *   - a zero shift returns the input unchanged (r0/r1 are never touched);
 *   - for 1..31 the low word is `(lo >> n) | (hi << (32 - n))` and the high
 *     word is `hi >> n`;
 *   - for >= 32 the low word becomes `hi >> (n - 32)` and the high word 0.
 * The `>= 32` path is selected by `32 - n <= 0`, so a count of exactly 32
 * takes it with a shift distance of zero, avoiding the undefined `>> 32`.
 *
 * Uncertainty: the routine performs no masking of the count, so counts of 64
 * or more are not defined by the original code either.  The C below matches
 * the assembly for every count the assembly itself defines.
 */
typedef union {
    u64 whole;
    struct {
        u32 lo;
        u32 hi;
    } parts;
} SplitU64;
u64 Func_02005c08(u64 value, u32 count)
{
    SplitU64 in;
    SplitU64 out;
    s32 spare;
    u32 carry;
    u32 shifted;

    if (count == 0u) {
        return value;
    }
    in.whole = value;
    spare = 32 - (s32)count;
    if (spare <= 0) {
        spare = -spare;
        out.parts.hi = 0u;
        out.parts.lo = in.parts.hi >> spare;
    } else {
        carry = in.parts.hi << spare;
        shifted = in.parts.lo >> count;
        out.parts.hi = in.parts.hi >> count;
        out.parts.lo = shifted | carry;
    }
    return out.whole;
}
