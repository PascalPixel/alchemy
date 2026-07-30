typedef unsigned int u32;
typedef unsigned long long u64;

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
u64 Func_02005c08(u64 value, u32 count)
{
    u32 lo = (u32)value;
    u32 hi = (u32)(value >> 32);
    u32 resultLo;
    u32 resultHi;

    if (count == 0u) {
        return value;
    }

    if ((int)(32 - count) <= 0) {
        resultLo = hi >> (count - 32u);
        resultHi = 0u;
    } else {
        resultLo = (lo >> count) | (hi << (32u - count));
        resultHi = hi >> count;
    }

    return ((u64)resultHi << 32) | resultLo;
}
