#include "types.h"

/*
 * One colour channel of a hue ramp, in 16.16 degrees.  The hue is first
 * reduced into a single turn by Func_08097a10 with a step of 180 << 17,
 * i.e. 360.0, and the reduced angle then selects one of four segments of the
 * usual six-segment hue curve:
 *
 *   hue <  60   the channel ramps up   with hue
 *   hue < 180   the channel is at its peak
 *   hue < 240   the channel ramps down with 240 - hue
 *   otherwise   the channel is the caller's fallback
 *
 * Both ramp segments share the same tail - scale the peak by the segment
 * position, then divide by 60.0 - so they are written as one, reached by
 * `goto` from the two arms exactly as the code reaches 0x080979e0 from the
 * two arms of the compare chain.  The three callers at 0x0809796a,
 * 0x08097978 and 0x0809798a pass hues 120.0 apart, which is what makes this
 * the red, green and blue channel of one colour.
 *
 * Neither relocated helper is reached by a `bl` naming it, and neither is
 * retained assembly:
 *
 *   - `mov ip, pc ; bx r3` at 0x080979e0 calls the pooled 0x03000118, the
 *     ARM fixed-point multiply relocated into IWRAM.  `mov ip, pc` sets the
 *     return address to the halfword after the `bx`, 0x080979e4, which
 *     consumes r0 as that call's result in the same frame - the ordinary
 *     indirect call already established for 0x0800ebec.
 *   - `bl Func_080072f0` at 0x080979ec is the runtime's `_call_via_r3` thunk
 *     inside the table at 0x080072e4, so it is a call through the register
 *     loaded just before it, the pooled 0x0300013c ratio helper.
 *
 * r4 holds the reduced angle without being saved: this image's call-used-r4
 * convention, assigned before every use and never read live-in, not the
 * 0x08002dd8 shape.
 *
 * Excluded (22 bytes): the 2-byte `movs r0, r0` alignment filler at
 * 0x080979fa and the five-word pool at 0x080979fc holding 0x003bffff,
 * 0x03000118, 0xffc40000, 0xff4c0000 and 0x0300013c.  The single epilogue is
 * at 0x080979f4, reached by every arm.  Three direct callers, the first at
 * 0x0809796a.
 */

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_080979a4)(s32 a, s32 b);

/* 0x0300013c, the IWRAM ratio helper reached through the call-via-r3 thunk;
   it returns value / divisor in 16.16. */
typedef s32 (*Ratio_080979a4)(s32 divisor, s32 value);

s32 Func_08097a10(s32 value, s32 step);

s32 Func_080979a4(s32 hue, s32 fallback, s32 peak)
{
    Multiply_080979a4 multiply = (Multiply_080979a4)0x03000118;
    Ratio_080979a4 ratio = (Ratio_080979a4)0x0300013c;
    s32 angle = Func_08097a10(hue, 180 << 17);
    s32 position;

    if (angle <= (60 << 16) - 1) {
        position = angle;
        goto ramp;
    }

    if ((u32)(angle - (60 << 16)) < (u32)(120 << 16)) {
        return peak;
    }

    if ((u32)(angle - (180 << 16)) > (u32)((60 << 16) - 1)) {
        return fallback;
    }

    position = (240 << 16) - angle;

ramp:
    return ratio(60 << 16, multiply(peak, position));
}
