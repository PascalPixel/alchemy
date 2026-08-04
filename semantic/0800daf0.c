#include "types.h"

/*
 * Step one actor towards the destination its target record names, and report
 * that the actor is still busy.
 *
 * The destination is clamped into the world box the context at 0x03001e70
 * keeps at +0xec..+0xf8, inset by 0x780000 horizontally and 0x600000/0x400000
 * vertically, with the vertical bounds shifted by the target's own height at
 * +0x0c.  With no target record, or a target whose word at +0x00 is clear,
 * only the tick at +0x04 advances.
 *
 * The three words at +0x38, +0x3c and +0x40 are parked at 0x80000000 on every
 * pass.  A non-zero halfword at +0x64 means "snap": the actor is placed on the
 * destination outright.  Otherwise the owner measures the remaining distance -
 * whole units through the IWRAM square root at 0x030001d8, and, below 64, the
 * finer 16.16 form through Func_080045d4 - and moves by that distance divided
 * by eight, capped by the actor's speed at +0x30.  A remaining distance under
 * 0x4000 finishes the move by assignment; anything longer is split into x and
 * y components by the ratio helper at 0x0300013c and scaled back up by the
 * capped step.
 *
 * The height difference is applied whole while it is 0x8000 or less and a
 * quarter at a time above that, with the usual round-toward-zero.
 *
 * The bounds are computed from the target record before the null test in the
 * ROM and are only used after it, so they are written after the test here;
 * nothing else observes the order.
 *
 * Relocated helpers, none of them retained assembly: `bl Func_080072f0` at
 * 0x0800dbd0 is the runtime's `_call_via_r3` thunk inside 0x080072e4,
 * reaching the pooled 0x030001d8; `bl Func_08007310` at 0x0800dc4a and
 * 0x0800dc60 is the `_call_via_fp` thunk in the same table, reaching the
 * pooled 0x0300013c, the ratio helper - which returns its second argument
 * over its first; and the four `mov ip, pc ; bx rN` sequences at 0x0800dbfc,
 * 0x0800dc08, 0x0800dc54 and 0x0800dc68 call the pooled 0x03000118, the
 * IWRAM-relocated ARM fixed-point multiply, in the same frame, the reading
 * established for 0x0800ebec.  All are written as ordinary calls through
 * function pointers.
 *
 * The epilogue at 0x0800dcac ends `pop {r1} ; bx r1`, so r0 is the result and
 * it is the constant 1.  No inbound call; the address is published as a Thumb
 * pointer at 0x08013650, so the caller is unknown.
 */

/* 0x030001d8, the IWRAM square-root helper. */
typedef s32 (*SquareRoot_0800daf0)(s32 value);

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_0800daf0)(s32 a, s32 b);

/* 0x0300013c, the IWRAM ratio helper: second argument over first. */
typedef s32 (*Ratio_0800daf0)(s32 denominator, s32 numerator);

s32 Func_080045d4(s32 sum_of_squares);

struct Target_0800daf0 {
    s32 active;
    u8 unknown_04[4];
    s32 x;
    s32 height;
    s32 y;
};

struct Actor_0800daf0 {
    u8 unknown_00[4];
    u16 tick;
    u8 unknown_06[2];
    s32 x;
    s32 height;
    s32 y;
    u8 unknown_14[0x1c];
    s32 speed;
    u8 unknown_34[4];
    s32 parked_38;
    s32 parked_3c;
    s32 parked_40;
    u8 unknown_44[0x11];
    u8 flag_55;
    u8 unknown_56[0xe];
    s16 snap;
    u8 unknown_66[2];
    struct Target_0800daf0 *target;
};

s32 Func_0800daf0(struct Actor_0800daf0 *actor)
{
    SquareRoot_0800daf0 square_root = (SquareRoot_0800daf0)0x030001d8;
    Multiply_0800daf0 multiply = (Multiply_0800daf0)0x03000118;
    Ratio_0800daf0 ratio = (Ratio_0800daf0)0x0300013c;
    u8 *context;
    struct Target_0800daf0 *target;
    s32 minimum_x;
    s32 minimum_y;
    s32 maximum_x;
    s32 maximum_y;
    s32 destination_x;
    s32 destination_y;
    s32 destination_height;
    s32 whole_x;
    s32 whole_y;
    s32 offset_x;
    s32 offset_y;
    s32 offset_height;
    s32 distance;
    s32 step;

    context = *(u8 **)0x03001e70;
    target = actor->target;
    actor->flag_55 = 0;

    if (target == NULL || target->active == 0) {
        goto done;
    }

    minimum_x = *(s32 *)(context + 236) + 0x780000;
    minimum_y = *(s32 *)(context + 240) + target->height + 0x600000;
    maximum_x = *(s32 *)(context + 244) - 0x780000;
    maximum_y = *(s32 *)(context + 248) + target->height - 0x400000;

    destination_x = target->x;
    destination_height = target->height;
    destination_y = target->y;

    actor->parked_38 = (s32)0x80000000;
    actor->parked_3c = (s32)0x80000000;
    actor->parked_40 = (s32)0x80000000;

    if (destination_x < minimum_x) {
        destination_x = minimum_x;
    }
    if (destination_y < minimum_y) {
        destination_y = minimum_y;
    }
    if (destination_x > maximum_x) {
        destination_x = maximum_x;
    }
    if (destination_y > maximum_y) {
        destination_y = maximum_y;
    }

    if (actor->snap != 0) {
        actor->x = destination_x;
        actor->height = destination_height;
        actor->y = destination_y;
        goto done;
    }

    whole_x = destination_x - actor->x;
    if (whole_x < 0) {
        whole_x += 0xffff;
    }
    whole_x >>= 16;
    whole_y = destination_y - actor->y;
    if (whole_y < 0) {
        whole_y += 0xffff;
    }
    whole_y >>= 16;

    distance = square_root(whole_x * whole_x + whole_y * whole_y) << 16;

    offset_x = destination_x - actor->x;
    offset_height = destination_height - actor->height;
    offset_y = destination_y - actor->y;

    if (distance < 0x400000) {
        distance = Func_080045d4(multiply(offset_x, offset_x)
                                 + multiply(offset_y, offset_y));
    }

    step = distance;
    if (distance < 0) {
        step = distance + 7;
    }
    step >>= 3;
    if (step > actor->speed) {
        step = actor->speed;
    }

    if (distance < 0x4000) {
        actor->x = destination_x;
        actor->y = destination_y;
    } else {
        if (distance > step) {
            offset_x = multiply(ratio(distance, offset_x), step);
            offset_y = multiply(ratio(distance, offset_y), step);
        }
        actor->x += offset_x;
        actor->y += offset_y;
    }

    whole_y = offset_height;
    if (whole_y < 0) {
        whole_y = -whole_y;
    }
    if (whole_y > 0x8000) {
        if (offset_height < 0) {
            offset_height += 3;
        }
        offset_height >>= 2;
    }
    actor->height += offset_height;

done:
    actor->tick++;
    return 1;
}
