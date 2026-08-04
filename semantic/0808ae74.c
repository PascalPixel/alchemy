#include "types.h"

/*
 * Accumulate the chance of a random encounter for one step of travel and,
 * when the accumulator crosses the threshold, roll the encounter.
 *
 * The gate is a run of conditions that all return 0 - the flag at 351 forces
 * the roll instead of blocking it, the flags at 352 and 353 block it, a zero
 * area index blocks it, so does a non-zero halfword at 0x0200048c, and so
 * does an area whose table row has a zero base weight.  When the flag at 5 is
 * set the record it names is consulted and a value over 130 at +0x124 blocks
 * the roll as well.
 *
 * The step count from Func_080772c8 above the row's own threshold is clamped
 * into 0..5; any positive amount blocks the roll outright while the word at
 * 0x02000484 is set, and otherwise adds five to the base weight.  The
 * per-area half-range at context+0x1a8 is materialised once, from four draws
 * of Func_08004458 combined as (a - b) + (c - d) and halved with the usual
 * round-toward-zero, and reused until it is cleared.
 *
 * The chance itself is 0x100000 divided by (weight << 20) + (weight * 16 -
 * 16) * half, scaled by the caller's rate, and added to the accumulator at
 * 0x02000478.  Below the limit at context+0x1ac the owner returns 0; at or
 * above it the accumulator's half-range is cleared and the roll runs: the
 * eight weights at +0x14 of the row are summed, a draw of Func_08004458
 * scales that sum into a slot, and the row's halfword for that slot is the
 * result, after Func_0808b320 is told which area it came from.
 *
 * Two relocated helpers are reached without a `bl` naming them and neither is
 * retained assembly: `bl Func_080072f0` at 0x0808af76 is the runtime's
 * `_call_via_r3` thunk inside 0x080072e4, so it is a call through the pooled
 * 0x0300013c, the ratio helper - which returns its second argument over its
 * first; and `mov ip, pc ; bx r3` at 0x0808af80 calls the pooled 0x03000118,
 * the IWRAM-relocated ARM fixed-point multiply, `mov ip, pc` setting the
 * return address to 0x0808af84 in the same frame, the reading established for
 * 0x0800ebec.  Both are written as ordinary calls through function pointers.
 *
 * The branch at 0x0808ae98 enters the roll without having set r9, which is
 * saved and restored by the prologue but not written on that path.  That is
 * the frame-balanced live-in recorded for resource_3cd:04b0 - a close without
 * its open - and converts as an uninitialised local rather than a skip.
 *
 * The slot search can leave the index at 8, one past the eight halfwords at
 * +0x04, which reads the first two weight bytes as a halfword.  That is what
 * the row does, so the read is written through the row's own bytes rather
 * than as an array subscript that would not be able to express it.
 *
 * The epilogue at 0x0808affc ends `pop {r1} ; bx r1`, so r0 is the result.
 * Two direct callers, 0x0808b03a and 0x0808b052.
 */

/* 0x0300013c, the IWRAM ratio helper: second argument over first. */
typedef s32 (*Ratio_0808ae74)(s32 denominator, s32 numerator);

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_0808ae74)(s32 a, s32 b);

s32 Func_080770c0(s32 flag);
void *Func_08077008(s32 flag);
s32 Func_080772c8(void);
s32 Func_08004458(void);
void Func_0808b320(s32 area);

/* One 28-byte row of the encounter table at 0x0809c610. */
struct Area_0808ae74 {
    u16 base_weight;
    u16 step_threshold;
    u16 slots[8];
    u8 slot_weights[8];
};

s32 Func_0808ae74(s32 area, s32 rate)
{
    Ratio_0808ae74 ratio = (Ratio_0808ae74)0x0300013c;
    Multiply_0808ae74 multiply = (Multiply_0808ae74)0x03000118;
    u8 *context;
    struct Area_0808ae74 *row;
    s32 *half_range;
    s32 *accumulator = (s32 *)0x02000478;
    s32 half;
    s32 weight;
    s32 steps;
    s32 total;
    s32 chance;
    s32 slot;
    u16 result;
    s32 index;

    context = *(u8 **)0x03001ebc;

    if (Func_080770c0(351) != 0) {
        goto roll;
    }
    if (Func_080770c0(352) != 0) {
        return 0;
    }
    if (Func_080770c0(353) != 0) {
        return 0;
    }
    if (area == 0) {
        return 0;
    }
    if (*(s16 *)(0x02000240 + 147 * 4) != 0) {
        return 0;
    }

    row = (struct Area_0808ae74 *)(0x0809c610 + area * 28);
    weight = row->base_weight;
    if (weight == 0) {
        return 0;
    }

    if (Func_080770c0(5) != 0) {
        if (*(s32 *)((u8 *)Func_08077008(5) + 146 * 2) > 130) {
            return 0;
        }
    }

    steps = Func_080772c8() - row->step_threshold;
    if (steps < 0) {
        steps = 0;
    }
    if (steps > 5) {
        steps = 5;
    }
    if (steps > 0 && *(s32 *)(0x02000240 + 145 * 4) != 0) {
        return 0;
    }
    weight += steps * 5;

    half_range = (s32 *)(context + 212 * 2);
    half = *half_range;
    if (half == 0) {
        s32 a = Func_08004458();
        s32 b = Func_08004458();
        s32 c = Func_08004458();
        s32 d = Func_08004458();

        half = ((a - b) + c) - d;
        half = (half + (s32)((u32)half >> 31)) >> 1;
        *half_range = half;
    }

    total = (weight << 20) + (weight * 16 - 16) * half;
    chance = multiply(ratio(total, 0x100000), rate);
    *accumulator += chance;
    if (*accumulator < *(s32 *)(context + 214 * 2)) {
        return 0;
    }

roll:
    *(s32 *)(context + 212 * 2) = 0;

    total = 0;
    for (index = 0; index < 8; index++) {
        total += row->slot_weights[index];
    }
    if (total == 0) {
        return 0;
    }

    slot = (s32)((u32)(total * Func_08004458()) >> 16) - row->slot_weights[0];
    index = 0;
    if (slot >= 0) {
        do {
            index++;
            if (index > 7) {
                break;
            }
            slot -= row->slot_weights[index];
        } while (slot >= 0);
    }

    result = *(u16 *)((u8 *)row + 4 + index * 2);
    Func_0808b320(area);
    return result;
}
