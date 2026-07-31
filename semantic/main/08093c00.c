#include "types.h"

/*
 * Try to move the party's lead actor one tile in the direction it faces,
 * playing the whole bump-and-recover sequence when the way is blocked.
 *
 * Each attempt builds a probe point from the actor's position - x and y
 * rounded down to whole tiles and re-centred by 0x80000 - and pushes it one
 * half tile forward through Func_0800447c, then one whole tile.  A near probe
 * that Func_080091d8 rejects with 1 abandons the move and returns -1; a far
 * probe it rejects with anything non-zero returns the running result, which
 * is -1 until the last attempt sets it to 0.
 *
 * A clear far probe runs the transition: the actor's step parameters at +0x28,
 * +0x30 and +0x34 are set, bit 0 of the flag byte at +0x55 is cleared for the
 * duration, and the facing handed to Func_080091e0 has its own bit 0 cleared.
 * When the actor is riding - byte 1 at +0x54 - the facing comes from the mount
 * record at +0x50 instead of the default 1.  Func_08092158 re-aims the camera
 * at the probe's whole-tile coordinates.
 *
 * Terrain 207 or 205 under the destination adds the drop: the actor and its
 * shadow fall 0x10000 twice and rise 0x10000 twice around two waits, instead
 * of the flat six-frame wait.  The flag byte is then restored from the copy
 * taken before the loop.
 *
 * The context at 0x03001ebc, when present, accumulates the distance walked at
 * +0x1b4 by its own per-step scale at +0x1b0 times one tile.
 *
 * Func_080091b0 finally reports what the actor arrived on; 249 means the step
 * has to be taken again, so the whole attempt repeats from the top.
 *
 * `mov ip, pc ; bx r4` at 0x08093dc8 calls the pooled 0x03000118, the
 * IWRAM-relocated ARM fixed-point multiply, in the same frame - the reading
 * established for 0x0800ebec - and is written as an ordinary call through a
 * function pointer.  Nothing in the row is retained assembly.
 *
 * The epilogue at 0x08093e00 ends `pop {r1} ; bx r1`, so r0 is the result.
 * One direct caller, 0x0808d94c, and the address is also published as a Thumb
 * pointer at 0x0808a2a4.
 */

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_08093c00)(s32 a, s32 b);

void *Func_08092054(s32 party);
void Func_0800447c(s32 distance, s32 facing, s32 *point);
s32 Func_080091d8(void *actor, const s32 *point);
void Func_080916b0(void);
void Func_08009080(void *actor, s32 animation);
void Func_080030f8(s32 frames);
void Func_080f9010(s32 sound);
void Func_080091e0(void *actor, s32 facing);
void Func_08092158(s32 party, s32 x, s32 y);
s32 Func_08093af8(void *actor, s32 terrain);
void Func_08091750(void);
s32 Func_080091b0(s32 layer, s32 x, s32 y);

struct Actor_08093c00 {
    u8 unknown_00[6];
    u16 facing;
    s32 x;
    s32 height;
    s32 y;
    s32 shadow;
    u8 unknown_18[0xa];
    u8 layer;
    u8 unknown_23[5];
    s32 step_28;
    u8 unknown_2c[4];
    s32 step_30;
    s32 step_34;
    u8 unknown_38[0x18];
    void *mount;
    u8 riding;
    u8 flags;
};

s32 Func_08093c00(void)
{
    Multiply_08093c00 multiply = (Multiply_08093c00)0x03000118;
    struct Actor_08093c00 *actor;
    u8 *context;
    s32 probe[3];
    s32 facing;
    s32 saved_flags;
    s32 result;
    s32 heading;
    s32 marker;
    s32 drop;

    actor = (struct Actor_08093c00 *)Func_08092054(*(s32 *)(0x02000240 + 500));
    result = -1;
    facing = (0x2000 + actor->facing) & 0xc000;
    saved_flags = actor->flags;
    context = *(u8 **)0x03001ebc;
    heading = 1;

    for (;;) {
        probe[0] = (actor->x & (s32)0xfff00000) + 0x80000;
        probe[1] = actor->height;
        probe[2] = (actor->y & (s32)0xfff00000) + 0x80000;
        Func_0800447c(0x100000, facing, probe);
        if (Func_080091d8(actor, probe) == 1) {
            return -1;
        }

        probe[0] = (actor->x & (s32)0xfff00000) + 0x80000;
        probe[1] = actor->height;
        probe[2] = (actor->y & (s32)0xfff00000) + 0x80000;
        Func_0800447c(0x200000, facing, probe);
        if (Func_080091d8(actor, probe) != 0) {
            break;
        }

        if (actor->riding == 1) {
            heading = *((u8 *)actor->mount + 38);
        }

        Func_080916b0();
        Func_08009080(actor, 6);
        Func_080030f8(6);
        Func_080f9010(152);
        Func_08009080(actor, 7);
        actor->step_30 = 0x30000;
        actor->step_34 = 0x20000;
        actor->step_28 = 0x40000;
        actor->flags &= 126;
        Func_080091e0(actor, heading & 254);
        Func_08092158(*(s32 *)(0x02000240 + 500),
                      *(s16 *)((u8 *)probe + 2),
                      *(s16 *)((u8 *)probe + 10));
        Func_08009080(actor, 6);
        Func_080091e0(actor, heading);

        marker = Func_08093af8(actor, 207);
        if (marker == 0) {
            marker = Func_08093af8(actor, 205);
        }
        if (marker != 0) {
            Func_08009080((void *)marker, 7);
            drop = (s32)0xffff0000;
            actor->height += drop;
            actor->shadow += drop;
            Func_080030f8(2);
            actor->height += drop;
            actor->shadow += drop;
            Func_080030f8(10);
            drop = 0x10000;
            actor->height += drop;
            actor->shadow += drop;
            Func_080030f8(4);
            actor->height += drop;
            actor->shadow += drop;
        } else {
            Func_080030f8(6);
        }

        actor->flags = (u8)saved_flags;
        Func_08091750();

        if (context != NULL) {
            *(s32 *)(context + 436) +=
                multiply(*(s32 *)(context + 432), 0x200000);
        }

        if (Func_080091b0(actor->layer, probe[0], probe[2]) != 249) {
            result = 0;
            break;
        }

        Func_08009080(actor, 1);
        Func_080030f8(6);
    }

    return result;
}
