#include "types.h"

/* Resolved whole-owner call targets. */
extern s32 Func_03000380();

/*
 * Apply the resource's asymmetric RGB555 colour adjustment.
 *
 * Owner span: 0x02000ecc-0x02000f34.  Control jumps over the mask literal at
 * 0x02000f14 and rejoins at 0x02000f18 before the common return.
 */
u16 Func_02000ecc(u16 color, s32 adjustment)
{
    s16 red = (s16)(color & 31);
    s16 green = (s16)((color >> 5) & 31);
    s16 blue = (s16)((color >> 10) & 31);
    u32 packed;

    red = (s16)(red + Func_03000380(
        red,
        (s32)((u32)adjustment << 2)
    ));
    green = (s16)(green - Func_03000380(green, adjustment));
    blue = (s16)(blue - Func_03000380(blue, adjustment));

    /* Only the increasing channel is explicitly saturated by this owner. */
    if (red > 31)
        red = 31;

    packed = (u32)(s32)red;
    packed |= (u32)(s32)green << 5;
    packed |= (u32)(s32)blue << 10;
    return (u16)packed;
}
