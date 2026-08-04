#include "types.h"

/*
 * Build a scaled 3x4 matrix from a triple of Euler angles, a translation and
 * a per-row scale, and hand it to the IWRAM matrix loader at 0x030002c0.
 * This is 0x08004d2c with one extra multiply on every rotation term: each row
 * of the x-then-y-then-z product is scaled by its own factor - the first row
 * by scale[0], the second by scale[1], the third by scale[2] - and the last
 * three words are the caller's translation.
 *
 * Neither relocated helper is reached by a `bl` naming it, and neither is
 * retained assembly:
 *
 *   - the twenty-five `mov ip, pc ; bx r3` sequences from 0x08004ea4 to
 *     0x08004fac call 0x03000118, the ARM fixed-point multiply relocated into
 *     IWRAM, inline.  `mov ip, pc` sets the return address to the halfword
 *     after each `bx`, so each returns into the same frame and stack
 *     adjustment with the live registers unchanged, and every landing site
 *     consumes r0 as that call's result - the reading already established for
 *     0x0800ebec.  The `movs r0, r0` fillers in front of some of them are
 *     alignment.
 *   - `bl Func_080072f0` at 0x08004fc4 is the runtime's `_call_via_r3` thunk
 *     inside the table at 0x080072e4, i.e. a call through the pooled
 *     0x030002c0.
 *
 * Both are written below as ordinary calls through function pointers.  As at
 * 0x08004ab0 and 0x08004d2c the loader's second and third argument registers
 * are left undisturbed, so it is declared with the one argument this owner
 * sets up.
 *
 * r4 accumulates the compound terms without being saved: this image's
 * call-used-r4 convention, assigned before every use and never read live-in,
 * not the 0x08002dd8 shape.
 *
 * Excluded (10 bytes): the 2-byte `movs r0, r0` alignment filler at
 * 0x08004fda and the two-word pool at 0x08004fdc holding 0x03000118 and
 * 0x030002c0.  The single epilogue is at 0x08004fc8.  No direct caller;
 * published as a Thumb pointer at 0x0800025c, in the same table as the
 * pointers to 0x08004ab0 and 0x08004d2c.
 */

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_08004e54)(s32 a, s32 b);

/* 0x030002c0, the IWRAM matrix loader, reached through the call-via-r3
   thunk. */
typedef void (*LoadMatrix_08004e54)(const s32 *matrix);

s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);

void Func_08004e54(const s32 angles[3], const s32 translation[3],
                   const s32 scale[3])
{
    Multiply_08004e54 multiply = (Multiply_08004e54)0x03000118;
    LoadMatrix_08004e54 load = (LoadMatrix_08004e54)0x030002c0;
    s32 sin_x = Func_08002322(angles[0]);
    s32 cos_x = Func_0800231c(angles[0]);
    s32 sin_y = Func_08002322(angles[1]);
    s32 cos_y = Func_0800231c(angles[1]);
    s32 sin_z = Func_08002322(angles[2]);
    s32 cos_z = Func_0800231c(angles[2]);
    s32 row;
    s32 matrix[12];

    row = scale[0];
    matrix[0] = multiply(row, multiply(cos_y, cos_z));
    matrix[1] = multiply(row, multiply(cos_y, sin_z));
    matrix[2] = multiply(row, -sin_y);

    row = scale[1];
    matrix[3] = multiply(row, multiply(multiply(sin_x, sin_y), cos_z) -
                              multiply(cos_x, sin_z));
    matrix[4] = multiply(row, multiply(multiply(sin_x, sin_y), sin_z) +
                              multiply(cos_x, cos_z));
    matrix[5] = multiply(row, multiply(sin_x, cos_y));

    row = scale[2];
    matrix[6] = multiply(row, multiply(multiply(cos_x, sin_y), cos_z) +
                              multiply(sin_x, sin_z));
    matrix[7] = multiply(row, multiply(multiply(cos_x, sin_y), sin_z) -
                              multiply(sin_x, cos_z));
    matrix[8] = multiply(row, multiply(cos_x, cos_y));

    matrix[9] = translation[0];
    matrix[10] = translation[1];
    matrix[11] = translation[2];

    load(matrix);
}
