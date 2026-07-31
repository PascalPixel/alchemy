#include "types.h"

/*
 * Build a 3x4 rotation matrix from a triple of Euler angles and hand it to
 * the IWRAM matrix loader at 0x030002c0.  The nine rotation terms are the
 * usual x-then-y-then-z product; the translation column is zeroed.  Every
 * term is formed with the ARM fixed-point multiply relocated into IWRAM at
 * 0x03000118, so the sines and cosines stay in 16.16 throughout.
 *
 * Both relocated helpers are reached without a `bl`, and neither is retained
 * assembly:
 *
 *   - the sixteen `mov ip, pc ; bx r3` sequences call 0x03000118 inline.
 *     `mov ip, pc` sets the return address to the instruction after the
 *     `bx`, so each returns into the same frame with the live registers
 *     unchanged - the reading already established for 0x0800ebec.  The
 *     `movs r0, r0` fillers in front of some of them are alignment.
 *   - `bl Func_080072f0` at the end is the runtime's `_call_via_r3` thunk at
 *     0x080072f0, i.e. a call through the pooled 0x030002c0.
 *
 * Both are written below as ordinary calls through function pointers.  The
 * loader's second and third argument registers are left undisturbed by this
 * owner, so it is declared with the one argument the owner sets up; the same
 * helper is spelled with four at 0x08004bd4 and 0x08004c1c, where the owner
 * does set them.
 *
 * Reached only as a published Thumb pointer at 0x08000224.
 */

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_08004ab0)(s32 a, s32 b);

/* 0x030002c0, the IWRAM matrix loader, reached through the call-via-r3
   thunk. */
typedef void (*LoadMatrix_08004ab0)(const s32 *matrix);

s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);

void Func_08004ab0(const s32 angles[3])
{
    Multiply_08004ab0 multiply = (Multiply_08004ab0)0x03000118;
    LoadMatrix_08004ab0 load = (LoadMatrix_08004ab0)0x030002c0;
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

    matrix[9] = 0;
    matrix[10] = 0;
    matrix[11] = 0;

    load(matrix);
}
