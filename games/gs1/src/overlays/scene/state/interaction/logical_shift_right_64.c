/*
 * resource_3a7 soft-float library: 64-bit logical right shift.  The overlay
 * carries its own copy of this library.
 */
#include "types.h"
#include "scene.h"

/*
 * r0/r1 hold the low and high words and r2 the count -- the ordinary pair
 * placement for an unsigned long long, so it is spelled that way here.  A
 * count of 32 or more takes the `32 - n <= 0' path with a shift distance of
 * zero, avoiding an undefined `>> 32'; counts of 64 and over are unmasked
 * here and undefined in the original too.  The else arm's statement order --
 * carry, shifted low, high, then the OR -- is load-bearing, as is zeroing the
 * high half first in the other arm.
 */
typedef union {
    u64 whole;
    struct {
        u32 lo;
        u32 hi;
    } parts;
} SplitU64;
u64 Runtime_LogicalShiftRight64(u64 value, u32 count)
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
