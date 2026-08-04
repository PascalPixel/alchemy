#include "types.h"

/*
 * Move an object toward a target point and record which way it ended up
 * facing.
 *
 * The distance is measured twice.  First cheaply, from the three deltas taken
 * to whole units with the usual truncating shift - add 0xffff when negative,
 * then arithmetic-shift right by 16 - squared, summed and passed to the IWRAM
 * integer square root at 0x030001d8, the result lifted back into 16.16.  Only
 * when that comes out under 128 << 13 is the exact 16.16 distance worth
 * computing, and it is redone with the IWRAM multiply and Func_080045d4.
 *
 * Under 128 << 9 the object is simply placed on the target and its three
 * words at +0x38 are set to 128 << 24.  Otherwise, unless the byte at +0x58
 * is set, the step is eased: a threshold is formed from +0x34 and the square
 * of +0x30 through the ratio helper, the step is the distance less half the
 * threshold when the distance exceeds it and half the distance otherwise, and
 * each axis advances by its delta scaled by step / distance.
 *
 * The landing point is stored at +0x38, +0x3c and +0x40, and the facing byte
 * at +0x56 becomes 16, or 18 when the Z delta dominates the X delta, or - if
 * the byte at +0x55 is clear - 17 when the Y delta dominates the one that
 * won.
 *
 * Three relocated IWRAM helpers are reached without a `bl` naming them, and
 * none of it is retained assembly:
 *
 *   - `bl Func_080072f0` at 0x0800d19e, 0x0800d226 and 0x0800d244 is the
 *     runtime's `_call_via_r3` thunk inside the table at 0x080072e4, so each
 *     is a call through the register loaded just before it: 0x030001d8, the
 *     integer square root, and twice 0x0300013c, the ratio helper.
 *   - the seven `mov ip, pc ; bx r3` sequences at 0x0800d1c4, 0x0800d1d0,
 *     0x0800d1dc, 0x0800d21c, 0x0800d254, 0x0800d266 and 0x0800d274 call the
 *     pooled 0x03000118, the ARM fixed-point multiply.  `mov ip, pc` sets the
 *     return address to the halfword after each `bx`, and every landing site
 *     consumes r0 as that call's result in the same frame with the live
 *     registers intact - the ordinary indirect call already established for
 *     0x0800ebec.  The `movs r0, r0` fillers in front of some of them are
 *     alignment.
 *
 * r4 accumulates the squared sum and the eased coordinates without being
 * saved: this image's call-used-r4 convention, assigned before every use and
 * never read live-in, not the 0x08002dd8 shape.
 *
 * The object is the 0x70-byte record of 0x0800ebec; the fields are named here
 * from this owner's use of them.
 *
 * Excluded (18 bytes): the 2-byte `movs r0, r0` alignment filler at
 * 0x0800d2f2 and the four-word pool at 0x0800d2f4 holding 0x0000ffff,
 * 0x030001d8, 0x03000118 and 0x0300013c.  The single epilogue is at
 * 0x0800d2e4, reached from every arm.  Eleven direct callers, the first at
 * 0x0800da2e, and published as a Thumb pointer at 0x08009154.
 */

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_0800d14c)(s32 a, s32 b);

/* 0x0300013c, the IWRAM ratio helper reached through the call-via-r3 thunk;
   it returns value / divisor. */
typedef s32 (*Ratio_0800d14c)(s32 divisor, s32 value);

/* 0x030001d8, the IWRAM square-root helper, also reached through the
   call-via-r3 thunk; here it takes and returns whole units. */
typedef s32 (*SquareRoot_0800d14c)(s32 value);

struct Object_0800d14c {
    u8 unknown_00[8];
    s32 position_08[3];
    u8 unknown_14[0x1c];
    s32 speed_30;
    s32 rate_34;
    s32 landing_38[3];
    u8 unknown_44[0x11];
    u8 lock_55;
    u8 facing_56;
    u8 unknown_57;
    u8 mode_58;
    u8 unknown_59[0x17];
};

s32 Func_080045d4(s32 squared_sum);

void Func_0800d14c(struct Object_0800d14c *object, s32 x, s32 y, s32 z)
{
    Multiply_0800d14c multiply = (Multiply_0800d14c)0x03000118;
    Ratio_0800d14c ratio = (Ratio_0800d14c)0x0300013c;
    SquareRoot_0800d14c square_root = (SquareRoot_0800d14c)0x030001d8;
    s32 dx = x - object->position_08[0];
    s32 dy = y - object->position_08[1];
    s32 dz = z - object->position_08[2];
    s32 distance;
    s32 span_x;
    s32 span_y;
    s32 span_z;

    if (dx < 0) {
        dx += 0xffff;
    }
    if (dy < 0) {
        dy += 0xffff;
    }
    if (dz < 0) {
        dz += 0xffff;
    }
    dx >>= 16;
    dy >>= 16;
    dz >>= 16;

    distance = square_root(dx * dx + dy * dy + dz * dz) << 16;

    if (distance < (128 << 13)) {
        span_x = x - object->position_08[0];
        span_y = y - object->position_08[1];
        span_z = z - object->position_08[2];
        distance = Func_080045d4(multiply(span_x, span_x) +
                                 multiply(span_y, span_y) +
                                 multiply(span_z, span_z));
    }

    if (distance < (128 << 9)) {
        object->position_08[0] = x;
        object->position_08[1] = y;
        object->position_08[2] = z;
        object->landing_38[0] = 128 << 24;
        object->landing_38[1] = 128 << 24;
        object->landing_38[2] = 128 << 24;
        return;
    }

    if (object->mode_58 == 0) {
        s32 threshold = ratio(object->rate_34,
                              multiply(object->speed_30, object->speed_30));
        s32 step;
        s32 share;

        if (distance > threshold) {
            step = distance - threshold / 2;
        } else {
            step = distance / 2;
        }

        share = ratio(distance, step);

        x = object->position_08[0] +
            multiply(x - object->position_08[0], share);
        y = object->position_08[1] +
            multiply(y - object->position_08[1], share);
        z = object->position_08[2] +
            multiply(z - object->position_08[2], share);
    }

    object->landing_38[0] = x;
    object->landing_38[1] = y;
    object->landing_38[2] = z;

    span_x = x - object->position_08[0];
    span_y = y - object->position_08[1];
    span_z = z - object->position_08[2];

    object->facing_56 = 16;

    if ((span_x < 0 ? -span_x : span_x) < (span_z < 0 ? -span_z : span_z)) {
        object->facing_56 = 18;
        span_x = span_z;
    }

    if (object->lock_55 != 0) {
        return;
    }

    if ((span_x < 0 ? -span_x : span_x) < (span_y < 0 ? -span_y : span_y)) {
        object->facing_56 = 17;
    }
}
