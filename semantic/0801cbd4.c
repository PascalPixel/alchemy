#include "types.h"

/*
 * Scale the RGB triple held at +0x576, +0x578 and +0x57a of a block and pack
 * it into a BGR555 colour - the same packer as 0x0801cc50, reading unsigned
 * halfwords out of a block at fixed offsets instead of a signed triple the
 * caller points at.  Each component is lifted into 16.16, scaled by its own
 * factor, taken back to whole units, clamped to 0..31, and placed in its
 * field of the returned halfword: red at bit 0, green at bit 5, blue at
 * bit 10.
 *
 * The three `mov ip, pc ; bx r4` sequences at 0x0801cbe4, 0x0801cbf8 and
 * 0x0801cc08 call the pooled 0x03000118, the ARM fixed-point multiply
 * relocated into IWRAM.  `mov ip, pc` sets the return address to the halfword
 * after each `bx` (0x0801cbe8, 0x0801cbfc, 0x0801cc0c), and every landing
 * site consumes r0 as that call's result in the same frame with the live
 * registers intact - the ordinary indirect call already established for
 * 0x0800ebec, not retained assembly.  The `movs r0, r0` at 0x0801cbf6 in
 * front of the second is alignment filler.
 *
 * The middle offset is built as 175 << 3 rather than pooled; the other two
 * are the pool words 0x00000576 and 0x0000057a.  r4 holds the pooled helper
 * address without being saved: this image's call-used-r4 convention,
 * assigned before its first use and never read live-in, not the 0x08002dd8
 * shape.
 *
 * Excluded (12 bytes): the three-word pool at 0x0801cc44 holding 0x00000576,
 * 0x03000118 and 0x0000057a; there is no alignment filler, the code ends
 * flush at 0x0801cc44.  The single epilogue is at 0x0801cc3e.  Seven direct
 * callers, the first at 0x0801cafc; no thumb pointer and no word reference.
 */

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_0801cbd4)(s32 a, s32 b);

struct Block_0801cbd4 {
    u8 unknown_000[0x576];
    u16 red_576;
    u16 green_578;
    u16 blue_57a;
};

u16 Func_0801cbd4(struct Block_0801cbd4 *block, s32 red_scale,
                  s32 green_scale, s32 blue_scale)
{
    Multiply_0801cbd4 multiply = (Multiply_0801cbd4)0x03000118;
    s32 red;
    s32 green;
    s32 blue;

    red = multiply(block->red_576 << 16, red_scale) >> 16;
    green = multiply(block->green_578 << 16, green_scale) >> 16;
    blue = multiply(block->blue_57a << 16, blue_scale) >> 16;

    if (red < 0) {
        red = 0;
    }
    if (green < 0) {
        green = 0;
    }
    if (blue < 0) {
        blue = 0;
    }
    if (red > 31) {
        red = 31;
    }
    if (green > 31) {
        green = 31;
    }
    if (blue > 31) {
        blue = 31;
    }

    return (u16)(red + (green << 5) + (blue << 10));
}
