#include "types.h"

/*
 * Project a world position onto the screen.  The caller's source position is
 * first run through the IWRAM transform at 0x03000250, which fills the
 * caller's three-word `projected` block with camera-space coordinates; the
 * depth in `projected[2]` is then range-checked against the camera block at
 * 0x03001ce0, and a rejected point leaves the block as the transform left it
 * and reports zero.  For an accepted point the perspective scale is either
 * the fixed 0x000151eb (when the camera's focal word is zero) or the quotient
 * the IWRAM helper at 0x030003f0 forms from the scaled focal length and the
 * shifted depth; x and y are scaled by it and offset by the camera's screen
 * centre.  The scale is the return value.
 *
 * Three of the calls here are indirect calls that the compiler spelled
 * without a `bl`:
 *
 *   - `bl Func_080072f0` and `bl Func_0800730c` are the runtime's
 *     `_call_via_r3` and `_call_via_sl` thunks at 0x080072e4, i.e. a call
 *     through the register loaded just before them;
 *   - `mov ip, pc ; bx r6` calls the pooled 0x03000118 inline, with
 *     `mov ip, pc` setting the return address to the instruction after the
 *     `bx`, so it resumes in the same frame with every live register intact.
 *
 * None of that is retained assembly; all three are written below as ordinary
 * calls through function pointers.
 *
 * 16 direct callers, the first at 0x0800b3ba, and published as a Thumb
 * pointer at 0x0800026c.
 */

/* 0x03000250, the IWRAM world-to-camera transform.  The owner sets up only
   the source and destination arguments, so it is declared with those two. */
typedef void (*Transform_08005268)(const s32 *source, s32 *projected);

/* 0x030003f0, the IWRAM division helper reached through the call-via-r3
   thunk. */
typedef s32 (*Divide_08005268)(s32 numerator, s32 denominator);

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_08005268)(s32 a, s32 b);

/* Camera block: focal word, the near and far depth limits, and the screen
   centre the projection is offset by. */
struct Camera_08005268 {
    s32 focal_00;
    s32 near_04;
    s32 far_08;
    s32 centre_x_0c;
    s32 centre_y_10;
};

s32 Func_08005268(const s32 *source, s32 *projected)
{
    Transform_08005268 transform = (Transform_08005268)0x03000250;
    Divide_08005268 divide = (Divide_08005268)0x030003f0;
    Multiply_08005268 multiply = (Multiply_08005268)0x03000118;
    struct Camera_08005268 *camera =
        (struct Camera_08005268 *)0x03001ce0;
    s32 depth;
    s32 scale;

    transform(source, projected);

    depth = -projected[2];
    if (depth < camera->near_04 || depth > camera->far_08) {
        return 0;
    }

    projected[2] = depth >> 16;

    if (camera->focal_00 == 0) {
        scale = 0x000151eb;
    } else {
        scale = divide(camera->focal_00 << 5, (u32)depth >> 11);
    }

    projected[0] = camera->centre_x_0c +
        multiply(projected[0], scale) / 0x10000;
    projected[1] = camera->centre_y_10 -
        multiply(projected[1], scale) / 0x10000;
    return scale;
}
