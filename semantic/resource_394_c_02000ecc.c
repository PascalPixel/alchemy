#include "types.h"

/* Resolved whole-owner call targets. */
/* One symbol PER CALL SITE, named at the site's PC-relative-decoded address
   (see resource_382:3ac for the rule, tools/bl_site_symbols.ts to derive
   them). All three reach the same ARM-mode IWRAM helper that scales a
   channel by the adjustment, and each still needs its own name. */
s32 Func_02001ee2();   /* 0x02000eec */
s32 Func_02001ef0();   /* 0x02000efa */
s32 Func_02001efe();   /* 0x02000f08 */

/*
 * Apply the resource's asymmetric RGB555 colour adjustment.
 *
 * Owner span: 0x02000ecc-0x02000f34.  Control jumps over the mask literal at
 * 0x02000f14 and rejoins at 0x02000f18 before the common return.
 */
u16 Func_02000ecc(u16 color, s32 adjustment)
{
    s16 green = (s16)((color >> 5) & 31);
    s16 red = (s16)(color & 31);
    s16 blue = (s16)((color >> 10) & 31);
    u32 packed;

    red = (s16)(red + Func_02001ee2(
        red,
        (s32)((u32)adjustment << 2)
    ));
    green = (s16)(green - Func_02001ef0(green, adjustment));
    blue = (s16)(blue - Func_02001efe(blue, adjustment));

    /* Only the increasing channel is explicitly saturated by this owner. */
    if (red > 31)
        red = 31;

    packed = (u32)(s32)red;
    packed |= (u32)(s32)green << 5;
    packed |= (u32)(s32)blue << 10;
    return (u16)packed;
}
