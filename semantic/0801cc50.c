#include "types.h"

/*
 * Scale a signed RGB triple and pack it into a BGR555 colour.  The three
 * components are the signed halfwords at +0, +2 and +4 of the caller's
 * triple; each is lifted into 16.16, scaled by its own factor, taken back to
 * whole units, clamped to 0..31, and placed in its field of the returned
 * halfword: red at bit 0, green at bit 5, blue at bit 10.
 *
 * The three `mov ip, pc ; bx r4` sequences at 0x0801cc60, 0x0801cc70 and
 * 0x0801cc80 call the pooled 0x03000118, the ARM fixed-point multiply
 * relocated into IWRAM.  `mov ip, pc` sets the return address to the halfword
 * after each `bx` (0x0801cc64, 0x0801cc74, 0x0801cc84), and every landing
 * site consumes r0 as that call's result in the same frame with the live
 * registers intact - the ordinary indirect call already established for
 * 0x0800ebec, not retained assembly.  The `movs r0, r0` at 0x0801cc5e,
 * 0x0801cc6e and 0x0801cc7e in front of them are alignment filler.
 *
 * r4 holds the pooled helper address without being saved: this image's
 * call-used-r4 convention, assigned before its first use and never read
 * live-in, not the 0x08002dd8 shape.
 *
 * Excluded (4 bytes): the one-word pool at 0x0801ccbc holding 0x03000118;
 * there is no alignment filler, the code ends flush at 0x0801ccbc.  The
 * single epilogue is at 0x0801ccb6.  Seven direct callers, the first at
 * 0x0801cd6a; no thumb pointer and no word reference.
 */

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_0801cc50)(s32 a, s32 b);

u16 Func_0801cc50(const s16 *colour, s32 red_scale, s32 green_scale,
                  s32 blue_scale)
{
    Multiply_0801cc50 multiply = (Multiply_0801cc50)0x03000118;
    s32 red;
    s32 green;
    s32 blue;

    red = multiply(colour[0] << 16, red_scale) >> 16;
    green = multiply(colour[1] << 16, green_scale) >> 16;
    blue = multiply(colour[2] << 16, blue_scale) >> 16;

    if (red < 0) {
        red = 0;
    }
    if (red > 31) {
        red = 31;
    }
    if (green < 0) {
        green = 0;
    }
    if (green > 31) {
        green = 31;
    }
    if (blue < 0) {
        blue = 0;
    }
    if (blue > 31) {
        blue = 31;
    }

    return (u16)(red + (green << 5) + (blue << 10));
}
