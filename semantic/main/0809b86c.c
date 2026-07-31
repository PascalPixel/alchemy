#include "types.h"

/*
 * Submit one sprite to the drawing helper at 0x08009008, after building its
 * placement block on the stack and rejecting placements that are off screen.
 *
 * Bit 2 of the flag byte at +0x47 selects the mirrored vertical origin: when
 * it is set, the block's Y offset becomes 254 << 17 minus the sprite's own Y,
 * otherwise it is zero.  The two words handed over as the second argument are
 * the sprite's own extents at +0x28 and +0x2c, each scaled by the target's
 * field at +0x18.  The placement block is the sprite's X, the offset just
 * computed, the sprite's Y plus that offset, and a zero.
 *
 * The submission is skipped unless the X is inside the horizontal window -
 * X + 0x001fffff must not exceed 0x012ffffe as an unsigned value - and the Y
 * lies strictly between 0xffe00000 and 224 << 16 as a signed value.
 *
 * The two `mov ip, pc ; bx r3` sequences at 0x0809b894 and 0x0809b8a0 call
 * the pooled 0x03000118, the ARM fixed-point multiply relocated into IWRAM.
 * `mov ip, pc` sets the return address to the halfword after each `bx`
 * (0x0809b898 and 0x0809b8a4), and both landing sites store r0 as that
 * call's result in the same frame with the live registers intact - the
 * ordinary indirect call already established for 0x0800ebec, not retained
 * assembly.  The `movs r0, r0` at 0x0809b892 and 0x0809b89e in front of them
 * are alignment filler.
 *
 * r4 holds the sprite pointer without being saved: this image's call-used-r4
 * convention, assigned before every use and never read live-in, not the
 * 0x08002dd8 shape.
 *
 * Excluded (16 bytes): the four-word pool at 0x0809b8e4 holding 0x03000118,
 * 0x001fffff, 0x012ffffe and 0xffe00000; there is no alignment filler, the
 * code ends flush at 0x0809b8e4.  The single epilogue is at 0x0809b8dc,
 * reached both by the rejections and by fall-through.  One direct caller, at
 * 0x0809b862; no thumb pointer and no word reference.
 */

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_0809b86c)(s32 a, s32 b);

struct Target_0809b86c {
    u8 unknown_00[0x18];
    s32 scale_18;
};

struct Sprite_0809b86c {
    struct Target_0809b86c *target_00;
    s32 x_04;
    s32 y_08;
    u8 unknown_0c[0x1c];
    s32 extent_x_28;
    s32 extent_y_2c;
    u8 unknown_30[0x17];
    u8 flags_47;
};

void Func_08009008(struct Target_0809b86c *target, s32 *placement,
                   s32 *extents, s32 limit);

void Func_0809b86c(struct Sprite_0809b86c *sprite)
{
    Multiply_0809b86c multiply = (Multiply_0809b86c)0x03000118;
    struct Target_0809b86c *target = sprite->target_00;
    s32 extents[2];
    s32 placement[4];
    s32 offset = 0;

    if ((sprite->flags_47 & 4) != 0) {
        offset = (254 << 17) - sprite->y_08;
    }

    extents[0] = multiply(sprite->extent_x_28, target->scale_18);
    extents[1] = multiply(sprite->extent_y_2c, target->scale_18);

    placement[0] = sprite->x_04;
    placement[1] = offset;
    placement[2] = sprite->y_08 + offset;
    placement[3] = 0;

    if ((u32)(sprite->x_04 + 0x001fffff) <= 0x012ffffeU &&
        sprite->y_08 > (s32)0xffe00000 &&
        sprite->y_08 < (224 << 16)) {
        Func_08009008(target, placement, extents, 0);
    }
}
