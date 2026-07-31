#include "types.h"

/*
 * Build a 3x4 matrix from a triple of Euler angles and a translation, and
 * hand it to the IWRAM matrix loader at 0x030002c0.  This is 0x08004ab0 with
 * a translation column: the nine rotation terms are the same x-then-y-then-z
 * product, formed with the same IWRAM multiply, and the last three words are
 * copied from the caller's translation instead of being zeroed.
 *
 * Neither relocated helper is reached by a `bl` naming it, and neither is
 * retained assembly:
 *
 *   - the sixteen `mov ip, pc ; bx r3` sequences from 0x08004d78 to
 *     0x08004e22 call 0x03000118 inline.  `mov ip, pc` sets the return
 *     address to the halfword after each `bx`, so each returns into the same
 *     frame and stack adjustment with the live registers unchanged, and every
 *     landing site consumes r0 as that call's result - the reading already
 *     established for 0x0800ebec.  The `movs r0, r0` fillers in front of some
 *     of them are alignment.
 *   - `bl Func_080072f0` at 0x08004e36 is the runtime's `_call_via_r3` thunk
 *     inside the table at 0x080072e4, i.e. a call through the pooled
 *     0x030002c0.
 *
 * Both are written below as ordinary calls through function pointers.  As at
 * 0x08004ab0 the loader's second and third argument registers are left
 * undisturbed, so it is declared with the one argument this owner sets up.
 *
 * r4 accumulates the compound terms without being saved: this image's
 * call-used-r4 convention, assigned before every use and never read live-in,
 * not the 0x08002dd8 shape.
 *
 * Excluded (8 bytes): the two-word pool at 0x08004e4c holding 0x03000118 and
 * 0x030002c0; there is no alignment filler, the code ends flush at
 * 0x08004e4c.  The single epilogue is at 0x08004e3a.  No direct caller;
 * published as a Thumb pointer at 0x08000254, in the same table as the
 * pointer to 0x08004ab0.
 */

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_08004d2c)(s32 a, s32 b);

/* 0x030002c0, the IWRAM matrix loader, reached through the call-via-r3
   thunk. */
typedef void (*LoadMatrix_08004d2c)(const s32 *matrix);

s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);

void Func_08004d2c(const s32 angles[3], const s32 translation[3])
{
    Multiply_08004d2c multiply = (Multiply_08004d2c)0x03000118;
    LoadMatrix_08004d2c load = (LoadMatrix_08004d2c)0x030002c0;
    s32 sin_x = Func_08002322(angles[0]);
    s32 cos_x = Func_0800231c(angles[0]);
    s32 sin_y = Func_08002322(angles[1]);
    s32 cos_y = Func_0800231c(angles[1]);
    s32 sin_z = Func_08002322(angles[2]);
    s32 cos_z = Func_0800231c(angles[2]);
    s32 matrix[12];

    matrix[0] = multiply(cos_y, cos_z);
    matrix[1] = multiply(cos_y, sin_z);
    matrix[2] = -sin_y;

    matrix[3] = multiply(multiply(sin_x, sin_y), cos_z) -
        multiply(cos_x, sin_z);
    matrix[4] = multiply(multiply(sin_x, sin_y), sin_z) +
        multiply(cos_x, cos_z);
    matrix[5] = multiply(sin_x, cos_y);

    matrix[6] = multiply(multiply(cos_x, sin_y), cos_z) +
        multiply(sin_x, sin_z);
    matrix[7] = multiply(multiply(cos_x, sin_y), sin_z) -
        multiply(sin_x, cos_z);
    matrix[8] = multiply(cos_x, cos_y);

    matrix[9] = translation[0];
    matrix[10] = translation[1];
    matrix[11] = translation[2];

    load(matrix);
}
