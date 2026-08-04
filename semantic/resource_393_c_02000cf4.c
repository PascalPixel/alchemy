#include "types.h"

/*
 * resource_393 owner at 0x02000cf4, 104 bytes: apply the asymmetric RGB555 colour
 * adjustment.
 *
 * TRANSPOSED from semantic/overlays/resource_394_c_02000ecc.c.  The two owners
 * are the same routine shared verbatim: over all 52 halfwords they differ in
 * exactly 3 places, and all three are BL halfwords.  No pool word differs.
 *
 * What was changed:
 *  - the entry symbol;
 *  - the calls, re-resolved with 'bun tools/overlay_call_targets.ts
 *    resource_393 0cf4': three sites, ONE distinct target, the veneer publishing
 *    the ARM-mode IWRAM helper Func_03000380.  The 394 source predates the
 *    corrected 'bl' rule and spelled the three sites as three different callees
 *    (Func_02001ee2 / Func_02001ef0 / Func_02001efe); they are one import, which
 *    is also what the code shape says - the same per-channel scale applied three
 *    times.  resource_394's own site resolves to the same import, so this is a
 *    correction inherited by the transposition rather than a per-overlay change.
 */

s32 Func_03000380();   /* ARM-mode IWRAM helper: scale a channel by the adjustment */

/*
 * Apply the resource's asymmetric RGB555 colour adjustment.
 *
 * Control jumps over a mask literal inside the span and rejoins before the
 * common return.
 */
u16 Func_02000cf4(u16 color, s32 adjustment)
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
