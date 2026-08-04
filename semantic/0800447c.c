#include "types.h"

/*
 * Step a horizontal position along a heading: add `distance` projected onto
 * the x and z axes to the first and third words of the caller's position
 * triple.  The y word is left alone.  The x term uses the sine of the heading
 * plus a quarter turn (0x4000 of the 0x10000-step circle), which is the
 * cosine; the z term uses the sine directly.
 *
 * The two `mov ip, pc ; bx r6` sequences are ordinary indirect calls through
 * the pooled pointer 0x03000118, the ARM fixed-point multiply relocated into
 * IWRAM.  `mov ip, pc` supplies the return address of the instruction after
 * the `bx`, so each returns into the same frame with the live registers
 * unchanged; the encoding is the only difference from `bl __call_via_r6`.
 * The `movs r0, r0` before the first is alignment filler.
 *
 * 105 direct callers, and published as a Thumb pointer at 0x0800012c.
 */

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_0800447c)(s32 a, s32 b);

s32 Func_08002322(s32 angle);

void Func_0800447c(s32 distance, s32 heading, s32 position[3])
{
    Multiply_0800447c multiply = (Multiply_0800447c)0x03000118;

    position[0] += multiply(distance, Func_08002322((128 << 7) + heading));
    position[2] += multiply(distance, Func_08002322(heading));
}
