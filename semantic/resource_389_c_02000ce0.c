#include "types.h"

/*
 * resource_389 owner at 0x02000ce0, 492 bytes: the scripted set-piece that runs
 * when the player's actor is standing on tile row 25.
 *
 * Reached from the overlay's entry-0 root (semantic/overlays/
 * resource_389_c_0200121c.c calls it at 0x02001364) and named as a callee there
 * before it was opened, which is why it was taken second in a root-down order.
 *
 * Complete owner: `push {r5, r6, lr}` plus the r8/r9/sl saves at 0x02000ce0
 * through the single epilogue at 0x02000eac.  That epilogue ends `pop {r0} /
 * bx r0`, so by the HANDOVER section-0 epilogue rule the owner is void.
 *
 * Pool map from a control-flow walk: 0x02000ce0-0x02000ebb is all reached as
 * instructions and 0x02000ebc-0x02000ecb is not (four words: 0x000fffff,
 * 0x02008bd1, 0x00009999, 0x0000e666).  476 code + 16 pool = 492, matching the
 * inventory row's code_bytes and span_bytes exactly.
 *
 * 0x02008bd1 is NOT a constant: under the proven 0x02008000 link base it is
 * file offset 0x0bd0 plus the Thumb bit, i.e. this overlay's own Func_02000bd0,
 * and the two sites that use it are Func_080000d0 (install task) at 0x02000d56
 * and Func_080000d8 (remove task) at 0x02000dd0.  That named 0x02000bd0 as a
 * per-frame task before it was disassembled.
 *
 * Calls: 45 sites, resolved with `bun tools/lib/overlay_call_targets.ts
 * resource_389 ce0`.  Per-target multiset, every site placed in program order:
 *   Func_0808a080  x10  0xcf0 0xd0c 0xd1c 0xd30 0xd88 0xd92 0xdc4 0xde2 0xdfe
 *                       0xe06
 *   Func_0808a010  x5   0xd4a 0xd72 0xd9c 0xdd6 0xe4c
 *   Func_080f9010  x4   0xd5c 0xdb2 0xdde 0xe52
 *   Func_080000f8  x3   0xde8 0xe12 0xe2c
 *   Func_0808a0f0  x3   0xd44 0xe7e 0xe88
 *   Func_080091e0  x2   0xd22 0xdca
 *   Func_080091f0  x2   0xe62 0xe70
 *   Func_0808a0e0  x2   0xd66 0xd7c
 *   Func_0808a0e8  x2   0xd6a 0xd82
 *   Func_080000d0  x1   0xd56
 *   Func_080000d8  x1   0xdd0
 *   Func_080091c0  x1   0xea4
 *   Func_080091e8  x1   0xd36
 *   Func_080091f8  x1   0xe74
 *   Func_080770c8  x1   0xe90
 *   Func_0808a018  x1   0xd00
 *   Func_0808a020  x1   0xea8
 *   Func_0808a090  x1   0xdaa
 *   Func_0808a0b8  x1   0xdbe
 *   Func_0808a158  x1   0xd2a
 *   Func_02000a68  x1   0xe46   (this overlay's own owner, prologue site)
 * Total 45; the inventory advertises 41, which is the safe direction (the
 * `calls` field is a FLOOR).
 *
 * Func_0808a020 at 0x02000ea8 is the shared tail: it is reached both by the
 * early `bne` at 0x02000d06 and by falling out of the body.  It is written once
 * after the `if`, which keeps its site count at one - duplicating it per arm
 * would have inflated the multiset.
 *
 * `x + 0x000fffff when negative, then asr #20` is the documented tile-grid
 * idiom (>>16 to integers, >>4 for the 16-pixel grid) with the usual
 * round-toward-zero bias added first.
 *
 * The three Func_080000f8 results are scaled by shift-and-add chains rather
 * than multiplies: rand*12>>16 (0..11 whole 16.16 units), rand*5>>16 then
 * *6553 (0..4 tenths of a 16.16 unit, 6553 = 0x10000/10, negated), and
 * rand*2>>16 (0 or 1).  The two pooled constants in the same family are
 * 0x00009999 = 0.6 and 0x0000e666 = 0.9 in 16.16.
 *
 * UNCERTAINTY: at 0x02000d00 r0 still holds the actor returned by the lookup at
 * 0x02000cf0 - nothing between them writes it - so Func_0808a018 is spelled
 * with that pointer.  Other overlays call the same import with no argument set;
 * whether it reads r0 is not established, only that the actor is what is in it
 * here.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
struct SceneActor;

struct SceneActor *Func_0808a080();  /* scene actor by selector */
void Func_0808a010();                /* wait this many frames */
void Func_0808a018();                /* scene bracket helper */
void Func_0808a020();                /* scene bracket close */
void Func_0808a090();                /* set an x/y scale pair, 16.16 */
void Func_0808a0b8();                /* two-argument actor call */
void Func_0808a0e0();                /* select an actor animation */
void Func_0808a0e8();                /* wait for that animation to finish */
void Func_0808a0f0();                /* place/aim the camera at a point */
void Func_0808a158();                /* two-argument actor call */
void Func_080091c0();                /* six-argument scripted-actor call */
void Func_080091e0();                /* set presentation mode */
void Func_080091e8();                /* set presentation flag */
void Func_080091f0();                /* three-argument fade/tint step */
void Func_080091f8();                /* commit the fade/tint step */
void Func_080770c8();                /* set an event flag */
void Func_080f9010();                /* play a cue by id */
s32 Func_080000f8();                 /* random source, no arguments */
void Func_080000d0();                /* install a task by entry and priority */
void Func_080000d8();                /* remove a task by entry */

/* This overlay's own seven-argument placement helper at 0x02000a68. */
void Func_02000a68();

struct SceneActor {
    u8 unknown_00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c */
    s32 z;                          /* 0x10 */
    u8 unknown_14[0x0e];
    u8 state22;                     /* 0x22 */
    u8 unknown_23[0x21];
    s32 field44;                    /* 0x44 */
    s32 field48;                    /* 0x48 */
};

void Func_02000ce0(void)
{
    struct SceneActor *actor = Func_0808a080(9);
    s32 tile = actor->x;

    if (tile < 0) tile += 0x000fffff;
    tile >>= 20;

    Func_0808a018(actor);

    if (tile == 25) {
        s32 spawnX;
        s32 spawnZ;
        s32 drift;
        s32 lift;
        struct SceneActor *target;
        struct SceneActor *anchor;

        Func_0808a080(11)->state22 = 1;
        Func_080091e0(Func_0808a080(11), 0);
        Func_0808a158(11, 14);
        Func_080091e8(Func_0808a080(11), 1);
        Func_0808a0f0(11, 0x019e0000, 0x00f00000);  /* 207 << 17, 240 << 16 */
        Func_0808a010(10);

        /* 0x02008bd1 = Func_02000bd0 + Thumb bit, this overlay's own task. */
        Func_080000d0((void *)0x02008bd1, 3200);    /* 200 << 4 */
        Func_080f9010(141);

        Func_0808a0e0(9, 1, 0);
        Func_0808a0e8(9);
        Func_0808a010(10);
        Func_0808a0e0(9, 2, 0);
        Func_0808a0e8(9);

        Func_0808a080(9)->field44 = 0;
        Func_0808a080(9)->field48 = 0x9999;         /* 0.6 in 16.16 */

        Func_0808a010(3);
        Func_0808a090(9, 0x00028000, 0x00004000);   /* 160 << 10, 128 << 7 */
        Func_080f9010(288);                         /* 144 << 1 */
        Func_0808a0b8(9, 416, 200);                 /* 208 << 1 */
        Func_080091e0(Func_0808a080(9), 0);

        /* The task installed above is removed again here. */
        Func_080000d8((void *)0x02008bd1);
        Func_0808a010(12);
        Func_080f9010(189);

        /* Whole 16.16 units, 0..11 columns east of the actor. */
        target = Func_0808a080(9);
        spawnX = target->x + (((Func_080000f8() * 12) >> 16) << 16);

        /*
         * Two DISTINCT lookups: 0x02000dfe keeps its result in sl for the y
         * argument below, 0x02000e06's is consumed immediately for z.  Folding
         * them into one would drop a call site.
         */
        anchor = Func_0808a080(9);
        spawnZ = Func_0808a080(9)->z + 0x00060000;  /* 192 << 11 */

        /* 0..4 tenths of a unit; 6553 = 0x10000 / 10.  Applied downwards. */
        drift = ((Func_080000f8() * 5) >> 16) * 6553;

        /* 0 or 1. */
        lift = (Func_080000f8() * 2) >> 16;

        Func_02000a68(spawnX, anchor->y, spawnZ, 0, -drift, lift, 0);

        Func_0808a010(20);
        Func_080f9010(154);
        Func_080091f0(0x00050000, 0x00050000, 0x00010000);  /* 5.0, 5.0, 1.0 */
        Func_080091f0(-1, -1, 0xe666);              /* 0.9 in 16.16 */
        Func_080091f8();

        Func_0808a0f0(9, 0, 0);
        Func_0808a0f0(11, 0, 0);

        Func_080770c8(768);                         /* set event flag 192 << 2 */
        Func_080091c0(21, 45, 4, 2, 21, 11);
    }

    Func_0808a020();
}
