#include "types.h"

/*
 * resource_3af owner at 0x020008e0, 88 bytes: a per-record blink timer.
 * It takes a record pointer, counts a byte down once per call, and when
 * that byte reaches zero picks a new facing from a random draw and
 * reloads the counter. It always returns 1.
 *
 * PUBLISHED, NOT CALLED -- its Thumb pointer 0x020090e1 (image offset
 * plus the 0x8000 link base) sits at 0x02004990 in a script-record
 * table. No `bl` in the image reaches it, so neither the call-graph
 * sweep nor `tools/overlay_unindexed.ts` lists it. See
 * resource_3af_c_02000b98.c for the sweep that does find this class.
 *
 * Two things in the body are worth naming rather than transcribing
 * blind:
 *
 * 1. The decrement is spelled `adds r3,#255`. r3 is written back with
 *    `strb`, so adding 255 to a byte and truncating is a subtraction of
 *    one. It is a decrement, not an increment by 255.
 *
 * 2. Both random draws are the standard scale-by-multiply-and-shift, and
 *    the multipliers are built from shift/add chains rather than
 *    literals:
 *      `r*4 + r = r*5`, `(r*5)<<4 = r*80`, `r*80 - r*5 = r*75`,
 *      `(r*75)<<2 = r*300`, then `>>16`  ->  a value in [0, 300)
 *      `r*4 + r = r*5`, `(r*5)<<4 = r*80`, then `>>16`, then `+80`
 *                                          ->  a value in [80, 160)
 *    Reading the intermediate `subs r3,r3,r2` as a subtraction of two
 *    separate quantities instead of part of one multiply is the easy
 *    mistake here.
 *
 * The facing written to the u16 at record +6 is 0xd000 above 200,
 * 0x5000 above 100, and otherwise zero -- that last store is
 * `strh r7`, and r7 still holds the counter byte, which is provably
 * zero on this path because it is the branch that tested it against
 * zero. Known-zero register reuse, the same shape as 0x02001684's.
 * Both compares are `bls`, so the threshold tests are unsigned.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x020008e0 through
 * `pop {r5, r6, r7} / pop {r1} / bx r1` at 0x02000930-0x02000934 -- note
 * the return is through r1, not r0, because r0 carries the return value
 * 1. No literal pool; the next owner (0x02000938, already tracked
 * byte-exact) begins at 0x02000938.
 *
 * Uncertainty: record +98 is the counter and +6 the facing by shape;
 * the constants 300, 80 and the two thresholds are exact.
 */

extern s32 Func_080000f8(void);

s32 Func_020008e0(u8 *record)
{
    u32 countdown;
    u32 draw;

    s32 permuted_3;
    permuted_3 = record[98];
    if (countdown != 0) {
        countdown += 255;               /* stored as a byte: a decrement */
    } else {
        draw = ((u32)Func_080000f8() * 300) >> 16;
        if (draw > 200) {
            *(u16 *)(record + 6) = (u16)(208 << 8);
        } else if (draw > 100) {
            *(u16 *)(record + 6) = (u16)(160 << 7);
        } else {
            *(u16 *)(record + 6) = 0;   /* the counter, known zero here */
        }
        countdown = ((((u32)Func_080000f8() * 80) >> 16) + 80);
    }
    countdown  = permuted_3;
    record[98] = (u8)countdown;
    return 1;
}
