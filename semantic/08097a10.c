#include "types.h"

/*
 * Reduce a 16.16 value modulo a 16.16 step.  A zero step yields zero; a step
 * whose top nibble is set is negated first, so the reduction always runs
 * against a positive magnitude.  The quotient comes back from the IWRAM
 * ratio helper, is truncated to whole steps by dropping its fractional half
 * word, scaled back up by the step and subtracted from the value.
 *
 * Neither relocated helper is reached by a `bl` naming it, and neither is
 * retained assembly:
 *
 *   - `bl Func_080072f0` at 0x08097a2e is the runtime's `_call_via_r3` thunk
 *     inside the table at 0x080072e4, so it is a call through the register
 *     loaded just before it, the pooled 0x0300013c ratio helper.
 *   - `mov ip, pc ; bx r4` at 0x08097a3c calls the pooled 0x03000118, the
 *     ARM fixed-point multiply relocated into IWRAM.  `mov ip, pc` sets the
 *     return address to the halfword after the `bx`, 0x08097a40, which
 *     consumes r0 as that call's result in the same frame - the ordinary
 *     indirect call already resolved at 0x0800ebec, not retained assembly.
 *     The `movs r0, r0` at 0x08097a3a in front of it is alignment filler.
 *
 * The ratio helper divides its second argument by its first: the only caller,
 * 0x080979a4, passes 180 << 17 - 360.0 in 16.16 - as the step, and uses the
 * result as an angle reduced into one turn.
 *
 * Excluded (12 bytes): the three-word pool at 0x08097a48 holding 0x0300013c,
 * 0xffff0000 and 0x03000118.  The epilogue is at 0x08097a42, shared by the
 * zero-step early-out and by fall-through.  One direct caller, at
 * 0x080979ae.
 */

/* 0x0300013c, the IWRAM ratio helper reached through the call-via-r3 thunk;
   it returns value / divisor in 16.16. */
typedef s32 (*Ratio_08097a10)(s32 divisor, s32 value);

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_08097a10)(s32 a, s32 b);

s32 Func_08097a10(s32 value, s32 step)
{
    Ratio_08097a10 ratio = (Ratio_08097a10)0x0300013c;
    Multiply_08097a10 multiply = (Multiply_08097a10)0x03000118;
    s32 whole;

    if (step == 0) {
        return 0;
    }

    if ((step & 0xf0000000) != 0) {
        step = -step;
    }

    whole = ratio(step, value) & 0xffff0000;
    return value - multiply(whole, step);
}
