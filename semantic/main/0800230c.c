#include "types.h"

/*
 * The boundary case shared by the sine and cosine lookup entry points.
 * There is no table index at an exact quadrant boundary: bit 14 selects
 * either zero or 1.0, and bit 13 supplies the sign.
 *
 * Complete Thumb leaf at 0x0800230c..0x0800231c (16 bytes).  Its only
 * in-image branch predecessor is the zero-index path in Func_0800231c.
 */
s32 Func_0800230c(s32 angle)
{
    u32 phase = (u32)angle;
    s32 value = (s32)(((phase >> 14) & 1) << 16);

    return (phase & 0x2000) != 0 ? -value : value;
}
