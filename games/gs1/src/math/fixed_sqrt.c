/*
 * Fixed-point square root: hand the value to the relocated routine at
 * 0x030001d8 and shift its result up by eight.
 */
#include "types.h"
#include "fixed_math.h"

/*
 * Func_080072f0 names a `bx rN` slot, so the call is indirect through the
 * register that slot selects; the trailing argument is the callee address
 * loaded into that register, not a parameter of the callee.
 */
s32 Func_080072f0(s32, s32, s32, s32);

/*
 * The callee takes one argument and returns one. Callers feed it a sum of
 * squares and use the result as a length, which reads as a square root but
 * is not established.
 */
s32 FixedSqrt(s32 value, s32 unused1, s32 unused2)
{
    return (s32)((u32)Func_080072f0(value, unused1, unused2, 0x030001D8) << 8);
}
