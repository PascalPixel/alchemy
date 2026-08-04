#include "types.h"

/*
 * Build the 160 per-scanline entries of a horizon effect.  The reference
 * point given by the caller is transformed once into camera space by the
 * IWRAM routine at 0x03000250; from it the loop derives, for each scanline,
 * the ratio between the camera height at 0x03001ce0 and the scanline's
 * distance below the horizon, and turns that into the entry's scroll offset
 * and rotation.  Scanlines at or above the horizon - the ones whose ratio
 * comes out non-negative - get a zeroed entry.  The last two words of every
 * entry are cleared unconditionally.
 *
 * Four relocated IWRAM helpers are reached without a `bl` naming them, and
 * none of it is retained assembly:
 *
 *   - `bl Func_080072f0` and `bl Func_0800730c` are the runtime's
 *     `_call_via_r3` and `_call_via_sl` thunks at 0x080072e4, so they are
 *     calls through the register loaded just before them: 0x03000250, the
 *     world-to-camera transform, and 0x0300013c, the ratio helper.
 *   - `bl Func_080072ec` is the `_call_via_r2` thunk, here reaching
 *     0x030001d8, the square-root helper.
 *   - the six `mov ip, pc ; bx r6` sequences call the pooled 0x03000118, the
 *     ARM fixed-point multiply, inline: `mov ip, pc` sets the return address
 *     to the instruction after the `bx`, so control resumes in the same
 *     frame with every live register intact.  That is the reading already
 *     established for 0x0800ebec.
 *
 * All of them are written below as ordinary calls through function pointers.
 *
 * Two direct callers, 0x08010be0 and 0x0801137a.
 */

/* 0x03000250, the IWRAM world-to-camera transform.  The owner sets up only
   the source and destination arguments. */
typedef void (*Transform_080123f4)(const s32 *source, s32 *projected);

/* 0x0300013c, the IWRAM ratio helper reached through the call-via thunks. */
typedef s32 (*Ratio_080123f4)(s32 numerator, s32 denominator);

/* 0x030001d8, the IWRAM square-root helper. */
typedef s32 (*SquareRoot_080123f4)(s32 value);

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_080123f4)(s32 a, s32 b);

/* Camera block at 0x03001ce0; +0x00 is the height the horizon is measured
   from and +0x10 the scanline the horizon sits on. */
struct Camera_080123f4 {
    s32 height_00;
    u8 unknown_04[0xc];
    s32 horizon_10;
};

/* One scanline's entry; the loop writes 20 bytes per scanline. */
struct Scanline_080123f4 {
    s32 offset;
    s32 rotation;
    s32 zero_08;
    s32 zero_0c;
    s32 unused_10;
};

void Func_080123f4(s32 reference, const s32 *source,
                   struct Scanline_080123f4 *scanlines)
{
    Transform_080123f4 transform = (Transform_080123f4)0x03000250;
    Ratio_080123f4 ratio = (Ratio_080123f4)0x0300013c;
    SquareRoot_080123f4 square_root = (SquareRoot_080123f4)0x030001d8;
    Multiply_080123f4 multiply = (Multiply_080123f4)0x03000118;
    const struct Camera_080123f4 *camera =
        (const struct Camera_080123f4 *)0x03001ce0;
    s32 position[3];
    s32 projected[3];
    s32 depth;
    s32 height;
    s32 line;

    position[0] = source[0];
    position[1] = 0;
    position[2] = source[2];
    transform(position, projected);

    depth = projected[1] - multiply(projected[2], reference);
    height = -camera->height_00;

    for (line = 0; line <= 159; line++, scanlines++) {
        s32 distance;
        s32 span;
        s32 scale;
        s32 dx;
        s32 dy;

        distance = ratio(height, (camera->horizon_10 - line) << 16);
        span = distance - reference;
        if (span == 0) {
            span = 1;
        }
        scale = ratio(span, depth);

        if (scale < 0) {
            scanlines->offset =
                ratio(camera->height_00,
                      multiply(-scale, 128 << 8));

            dx = (projected[2] - scale) >> 4;
            dy = (multiply(scale, distance) - projected[1]) >> 4;

            scale = square_root(multiply(dx, dx) + multiply(dy, dy)) << 12;
            if (dy < 0) {
                scale = -scale;
            }
            scanlines->rotation = multiply(scale, 128 << 8);
        } else {
            scanlines->offset = 0;
            scanlines->rotation = 0;
        }

        scanlines->zero_08 = 0;
        scanlines->zero_0c = 0;
    }
}
